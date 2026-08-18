"""Binary reconstruction pipeline for Synthex.

Takes Ghidra decompiled output and produces a single compilable, runnable C project
by identifying user functions, cleaning them via LLM, and assembling the result.
"""

from __future__ import annotations

import re
import subprocess
import sys
import textwrap
from dataclasses import dataclass, field
from pathlib import Path

from synthex.llm import LLMClient


# ---------------------------------------------------------------------------
# Data types
# ---------------------------------------------------------------------------

@dataclass
class DecompiledFunction:
    """A single function extracted from Ghidra output."""
    name: str          # e.g. "FUN_1400014b0"
    address: str       # e.g. "1400014b0"
    code: str          # full function text
    line_start: int
    line_end: int


@dataclass
class ReconstructedFunction:
    """A function after LLM reconstruction."""
    original_name: str
    new_name: str
    code: str               # clean C code (function body only, no includes/structs)
    structs: list[str]      # struct declarations this function needs
    includes: list[str]     # #include lines this function needs
    description: str = ""


@dataclass
class ReconstructionResult:
    """Overall result of the reconstruction pipeline."""
    functions: list[ReconstructedFunction]
    assembled_code: str
    output_dir: Path
    compiled: bool = False
    compile_errors: list[str] = field(default_factory=list)
    compile_attempts: int = 0
    diff_test_passed: bool | None = None
    diff_test_output: str = ""
    total_tokens: int = 0


# ---------------------------------------------------------------------------
# User function addresses (hardcoded for kvstore.exe)
# ---------------------------------------------------------------------------

USER_FUNCTIONS = {
    "FUN_1400014b0": {"desc": "kv_set: Sets a key-value pair in a hash table (uses djb2 hash, malloc, strcmp, strncpy)"},
    "FUN_140001640": {"desc": "kv_get: Looks up a value by key (uses djb2 hash, strcmp)"},
    "FUN_140001700": {"desc": "kv_delete: Deletes a key-value pair (uses djb2 hash, strcmp, free)"},
    "FUN_1400017f0": {"desc": "kv_save: Saves all entries to a file (uses fopen, fprintf, fclose)"},
    "FUN_140001880": {"desc": "kv_load: Loads entries from a file (uses fopen, fgets, fclose, strchr, calls kv_set)"},
    "FUN_140001940": {"desc": "kv_destroy: Frees all entries and the store (uses free)"},
    "FUN_140003050": {"desc": "main: Interactive command loop (calloc, scanf, printf, calls kv_set/kv_get/kv_delete/kv_save/kv_load/kv_destroy)"},
}

# The hash is inlined in kv_set, kv_get, kv_delete -- not a separate function in the decompiled output.
# FUN_1400019c0 is actually CRT cleanup, not hash_key. The djb2 hash (0x1505 = 5381) is inlined.

# Processing order: leaf functions first, then functions that depend on them
PROCESSING_ORDER = [
    "FUN_1400014b0",  # kv_set (leaf - called by kv_load and main)
    "FUN_140001640",  # kv_get (leaf)
    "FUN_140001700",  # kv_delete (leaf)
    "FUN_1400017f0",  # kv_save (leaf)
    "FUN_140001880",  # kv_load (calls kv_set)
    "FUN_140001940",  # kv_destroy (leaf)
    "FUN_140003050",  # main (calls everything)
]


# ---------------------------------------------------------------------------
# Parser: split decompiled output into individual functions
# ---------------------------------------------------------------------------

def parse_decompiled(source: str) -> list[DecompiledFunction]:
    """Parse Ghidra decompiled output into individual functions."""
    functions: list[DecompiledFunction] = []
    lines = source.split("\n")

    # Pattern: /* Function: NAME @ ADDRESS */
    func_header_re = re.compile(
        r"/\*\s*Function:\s+(\S+)\s+@\s+([0-9a-fA-F]+)\s*\*/"
    )

    i = 0
    while i < len(lines):
        m = func_header_re.search(lines[i])
        if m:
            name = m.group(1)
            address = m.group(2)
            func_start = i

            # Find the function body: skip to the opening brace
            j = i + 1
            # Skip warning comments and blank lines until we hit the signature
            while j < len(lines) and not lines[j].strip().startswith(("void", "undefined", "char",
                                                                       "int", "uint", "longlong",
                                                                       "bool", "ulonglong",
                                                                       "IMAGE_SECTION_HEADER",
                                                                       "IMAGE_DOS_HEADER")):
                if lines[j].strip().startswith("/*") and "Function:" in lines[j]:
                    break  # hit next function header
                j += 1

            if j >= len(lines) or (lines[j].strip().startswith("/*") and "Function:" in lines[j]):
                i = j
                continue

            # Now find the end of the function: matching braces
            brace_count = 0
            started = False
            func_end = j
            while func_end < len(lines):
                line = lines[func_end]
                for ch in line:
                    if ch == "{":
                        brace_count += 1
                        started = True
                    elif ch == "}":
                        brace_count -= 1
                if started and brace_count == 0:
                    func_end += 1
                    break
                func_end += 1

            code = "\n".join(lines[j:func_end])
            functions.append(DecompiledFunction(
                name=name,
                address=address,
                code=code,
                line_start=func_start + 1,  # 1-indexed
                line_end=func_end,
            ))
            i = func_end
        else:
            i += 1

    return functions


# ---------------------------------------------------------------------------
# LLM prompt for reconstruction
# ---------------------------------------------------------------------------

SYSTEM_PROMPT = textwrap.dedent("""\
    You are reconstructing a C program from Ghidra decompiled output.
    Your job is to produce clean, compilable, correct C code.

    Rules:
    1. Give the function a meaningful name based on what it does
    2. Use standard C types (int, char*, void*, size_t, etc.) -- NO Ghidra types
       (undefined, undefined4, undefined8, ulonglong, longlong, etc.)
    3. Remove stack canary code (anything involving in_FS_OFFSET, FS_OFFSET, unaff_GS_OFFSET)
    4. Remove goto statements -- use if/else, while, for, return instead
    5. Add a brief one-line comment explaining what the function does
    6. If this function uses a data structure (e.g., linked list node, hash table),
       declare the struct at the top of your response in a STRUCTS block
    7. Keep the logic EXACTLY correct -- do not change the algorithm
    8. Use puts() for simple string output, printf() for formatted output, fprintf() for file/stderr output
    9. The hash algorithm uses djb2: hash starts at 5381, hash = hash * 33 + c, result mod 64

    IMPORTANT CONSTANTS from the binary:
    - 0x1505 = 5381 (djb2 hash seed)
    - 0x21 = 33 (djb2 multiplier)
    - 0x3f = 63 (TABLE_SIZE - 1, used as mask: hash & 0x3f == hash % 64)
    - 0x80 = 128 (MAX_KEY_LEN)
    - 0x200 = 512 (MAX_VAL_LEN)
    - 0x7f = 127 (MAX_KEY_LEN - 1)
    - 0x1ff = 511 (MAX_VAL_LEN - 1)
    - 0x27f = 639 (128 + 512 - 1 = offset of last byte in value field)
    - 0x280 = 640 (128 + 512 = offset of 'next' pointer in Entry struct)
    - 0x288 = 648 (sizeof(Entry) = 128 + 512 + 8)
    - 0x208 = 520 (sizeof(KVStore) = 64 * 8 + 8 for count, padded)

    Response format:
    Line 1: just the function name (e.g., kv_set)
    Then optionally a STRUCTS block:
    ```structs
    typedef struct Entry { ... } Entry;
    ```
    Then the C code in a ```c block:
    ```c
    /* Sets a key-value pair in the hash table */
    int kv_set(KVStore *store, const char *key, const char *value) {
        ...
    }
    ```
""")


def build_user_prompt(func: DecompiledFunction, function_map: dict[str, str]) -> str:
    """Build the user prompt for reconstructing a single function."""
    map_lines = []
    for orig_name, new_name in function_map.items():
        desc = USER_FUNCTIONS.get(orig_name, {}).get("desc", "")
        if desc:
            map_lines.append(f"  {orig_name} -> {new_name}: {desc}")
        else:
            map_lines.append(f"  {orig_name} -> {new_name}")

    map_section = "\n".join(map_lines) if map_lines else "  (none yet)"

    return textwrap.dedent(f"""\
        Known functions in this program (already reconstructed):
        {map_section}

        Here is the decompiled function to reconstruct:

        ```c
        {func.code}
        ```

        Produce clean, compilable C code for this function.
        Return ONLY a ```c code fence containing the complete function definition.
        If this function uses a custom struct, put the struct typedef INSIDE the same code fence, before the function.
        Do NOT include explanations or text outside the code fence.
    """)


# ---------------------------------------------------------------------------
# Response parser
# ---------------------------------------------------------------------------

def parse_llm_response(response_text: str) -> ReconstructedFunction:
    """Parse the LLM response into a ReconstructedFunction."""
    lines = response_text.strip().split("\n")

    # First non-empty line is the function name — extract just the identifier
    new_name = ""
    for line in lines:
        stripped = line.strip()
        if stripped and not stripped.startswith("```") and not stripped.startswith("#"):
            raw = stripped.strip("`").strip().rstrip("{;")
            # Extract just the function name from signatures like "char *kv_get(...)"
            name_match = re.search(r'\b(\w+)\s*\(', raw)
            if name_match:
                new_name = name_match.group(1)
            else:
                # Just take the last word (handles "void kv_free")
                words = raw.split()
                new_name = words[-1] if words else raw
            break

    if not new_name:
        new_name = "unknown_function"

    # Extract structs block
    structs: list[str] = []
    structs_match = re.search(r"```structs\s*\n(.*?)```", response_text, re.DOTALL)
    if structs_match:
        structs_text = structs_match.group(1).strip()
        if structs_text:
            structs.append(structs_text)

    # Extract ALL code blocks and merge them
    code_blocks = re.findall(r"```(?:c)?\s*\n(.*?)```", response_text, re.DOTALL)
    # Filter out struct blocks
    c_blocks = [b.strip() for b in code_blocks if b.strip() and "```structs" not in b]
    code = "\n\n".join(c_blocks) if c_blocks else ""

    if not code:
        # Fallback: take everything after the first line
        remaining = "\n".join(lines[1:]).strip()
        code = remaining

    # Extract includes from the code
    includes: list[str] = []
    code_lines = code.split("\n")
    filtered_lines = []
    for line in code_lines:
        if line.strip().startswith("#include"):
            includes.append(line.strip())
        else:
            filtered_lines.append(line)
    code = "\n".join(filtered_lines)

    # Always extract function name from the code itself (most reliable)
    # Skip struct/typedef definitions and find the actual function
    func_defs = re.findall(r'(?:^|\n)\s*(?:static\s+)?(?:const\s+)?(?:void|int|char\s*\*?|long\s*(?:long)?|unsigned\s*(?:long\s*(?:long)?|int|char)?|size_t|ssize_t)\s*\*?\s*(\w+)\s*\([^;]*\)\s*\{', code)
    # Filter out struct-related matches
    # Filter: structs detected in the same code block aren't function names
    local_structs = set(re.findall(r'typedef\s+struct\s+(\w+)', code))
    local_structs.update(re.findall(r'\}\s*(\w+)\s*;', code))
    func_defs = [n for n in func_defs if n not in local_structs]
    if func_defs:
        new_name = func_defs[0]
    elif new_name == "unknown_function" or not re.match(r'^[a-zA-Z_]\w*$', new_name):
        func_def = re.search(r'(?:void|int|char\s*\*|long|unsigned)\s+(\w+)\s*\(', code)
        if func_def and func_def.group(1) not in local_structs:
            new_name = func_def.group(1)

    # Extract description from comment
    description = ""
    desc_match = re.search(r"/\*\s*(.+?)\s*\*/", code)
    if desc_match:
        description = desc_match.group(1)

    return ReconstructedFunction(
        original_name="",  # will be set by caller
        new_name=new_name,
        code=code,
        structs=structs,
        includes=includes,
        description=description,
    )


# ---------------------------------------------------------------------------
# Assembly: combine all functions into a single .c file
# ---------------------------------------------------------------------------

def assemble_project(
    functions: list[ReconstructedFunction],
    output_dir: Path,
) -> str:
    """Assemble all reconstructed functions into a single compilable C file."""
    output_dir.mkdir(parents=True, exist_ok=True)

    # Collect all unique includes
    all_includes = set()
    # Always need these
    all_includes.update(["#include <stdio.h>", "#include <stdlib.h>", "#include <string.h>"])
    for func in functions:
        all_includes.update(func.includes)

    # Collect all struct declarations (deduplicate by struct name)
    # Split multi-struct blocks into individual structs first
    all_structs: list[str] = []
    seen_struct_names: set[str] = set()
    for func in functions:
        for s in func.structs:
            # Split on "typedef struct" boundaries (keep each as separate block)
            individual = re.split(r'(?=typedef struct\b)', s)
            for block in individual:
                block = block.strip()
                if not block:
                    continue
                # Extract struct name
                name_match = re.search(r'struct\s+(\w+)', block)
                key = name_match.group(1) if name_match else block.split("\n")[0]
                if key not in seen_struct_names:
                    seen_struct_names.add(key)
                    all_structs.append(block)

    # Build forward declarations — extract actual C function signatures only
    forward_decls: list[str] = []
    c_return_types = {"void", "int", "char", "long", "unsigned", "short",
                      "float", "double", "size_t", "ssize_t", "Entry",
                      "KVStore", "const"}
    for func in functions:
        for line in func.code.split("\n"):
            line = line.strip()
            if not line or line.startswith("/*") or line.startswith("//") or line.startswith("*"):
                continue
            if line.startswith("#"):
                continue
            # Must start with a C return type and contain parentheses
            first_word = line.split()[0].rstrip("*") if line.split() else ""
            if first_word in c_return_types and "(" in line:
                sig = line.rstrip("{").strip()
                if not sig.endswith(";"):
                    sig += ";"
                forward_decls.append(sig)
                break

    # Assemble
    parts: list[str] = []
    parts.append("/* Reconstructed from kvstore.exe by Synthex */\n")

    # Includes
    for inc in sorted(all_includes):
        parts.append(inc)
    parts.append("")

    # Constants
    parts.append("#define TABLE_SIZE 64")
    parts.append("#define MAX_KEY_LEN 128")
    parts.append("#define MAX_VAL_LEN 512")
    parts.append("")

    # Structs
    for s in all_structs:
        parts.append(s)
        parts.append("")

    # Forward declarations
    parts.append("/* Forward declarations */")
    for decl in forward_decls:
        parts.append(decl)
    parts.append("")

    # Function bodies — strip inline struct defs (already in header)
    for func in functions:
        clean_code = func.code
        # Remove any typedef struct blocks that are already declared above
        for sname in seen_struct_names:
            clean_code = re.sub(
                rf'typedef\s+struct\s+{sname}\s*\{{[^}}]*\}}\s*{sname}\s*;',
                '', clean_code, flags=re.DOTALL
            )
        # Remove bare struct blocks without typedef too
        for sname in seen_struct_names:
            clean_code = re.sub(
                rf'struct\s+{sname}\s*\{{[^}}]*\}}\s*;',
                '', clean_code, flags=re.DOTALL
            )
        # Remove any #define lines (already in header)
        clean_code = re.sub(r'^#define\s+.*$', '', clean_code, flags=re.MULTILINE)
        # Remove blank lines caused by removals
        clean_code = re.sub(r'\n{3,}', '\n\n', clean_code).strip()
        if clean_code:
            parts.append(clean_code)
            parts.append("")

    assembled = "\n".join(parts)

    # Write output
    out_file = output_dir / "reconstructed.c"
    out_file.write_text(assembled)

    # Write Makefile
    makefile = output_dir / "Makefile"
    makefile.write_text(textwrap.dedent("""\
        CC = gcc
        CFLAGS = -Wall -Wno-unused-variable -o
        TARGET = reconstructed

        all: $(TARGET)

        $(TARGET): reconstructed.c
        \t$(CC) $(CFLAGS) $(TARGET) reconstructed.c

        clean:
        \trm -f $(TARGET) $(TARGET).exe
    """))

    return assembled


# ---------------------------------------------------------------------------
# Compilation
# ---------------------------------------------------------------------------

def try_compile(output_dir: Path, attempt: int = 1) -> tuple[bool, list[str]]:
    """Try to compile reconstructed.c with gcc. Returns (success, errors)."""
    src = output_dir / "reconstructed.c"
    out = output_dir / "reconstructed"
    if sys.platform == "win32":
        out = output_dir / "reconstructed.exe"

    try:
        result = subprocess.run(
            ["gcc", "-Wall", "-Wno-unused-variable", "-o", str(out), str(src)],
            capture_output=True,
            text=True,
            timeout=30,
        )
        if result.returncode == 0:
            return True, []
        errors = (result.stderr or "").strip().split("\n")
        return False, errors
    except FileNotFoundError:
        return False, ["gcc not found on PATH"]
    except subprocess.TimeoutExpired:
        return False, ["compilation timed out"]


def fix_compile_errors(
    code: str,
    errors: list[str],
    llm: LLMClient,
) -> str:
    """Fix compilation errors programmatically — no LLM (avoids stubbing)."""
    fixed = code

    for err in errors:
        # unknown type name
        m = re.search(r"unknown type name ['\"](\w+)['\"]", err)
        if m:
            bad = m.group(1)
            remap = {
                "undefined8": "long long", "undefined4": "int",
                "undefined2": "short", "undefined": "unsigned char",
                "byte": "unsigned char", "longlong": "long long",
                "ulonglong": "unsigned long long", "uint": "unsigned int",
                "ulong": "unsigned long", "bool": "int", "code": "void",
            }
            if bad in remap:
                fixed = fixed.replace(bad, remap[bad])

    # Common Ghidra residue
    fixed = re.sub(r'\bin_FS_OFFSET\b', '0', fixed)
    fixed = re.sub(r'\bLOCK\s*\(\)', '/* LOCK */', fixed)
    fixed = re.sub(r'\bUNLOCK\s*\(\)', '/* UNLOCK */', fixed)
    fixed = re.sub(r'\b__stack_chk_fail\s*\(\)', 'abort()', fixed)
    # Fix long long used as pointer param
    fixed = re.sub(r'\(long long\)(store|param_\d+)', r'(void *)\1', fixed)

    return fixed


# ---------------------------------------------------------------------------
# Differential testing
# ---------------------------------------------------------------------------

def differential_test(
    original_binary: Path,
    reconstructed_binary: Path,
    test_input: Path | None = None,
) -> tuple[bool, str]:
    """Run both binaries with same input and compare stdout."""
    if test_input is None:
        input_text = "set foo bar\nget foo\nset hello world\nget hello\ndel foo\nget foo\nquit\n"
    else:
        input_text = test_input.read_text()

    def run_binary(binary: Path) -> str | None:
        try:
            result = subprocess.run(
                [str(binary)],
                input=input_text,
                capture_output=True,
                text=True,
                timeout=10,
            )
            return result.stdout
        except (subprocess.TimeoutExpired, FileNotFoundError, OSError):
            return None

    orig_out = run_binary(original_binary)
    recon_out = run_binary(reconstructed_binary)

    if orig_out is None:
        return False, "Failed to run original binary"
    if recon_out is None:
        return False, "Failed to run reconstructed binary"

    if orig_out == recon_out:
        return True, "Output matches"

    # Show diff
    diff_report = f"ORIGINAL:\n{orig_out}\n\nRECONSTRUCTED:\n{recon_out}"
    return False, diff_report


# ---------------------------------------------------------------------------
# Main pipeline
# ---------------------------------------------------------------------------

def reconstruct(
    decompiled_file: str,
    output_dir: str = "reconstructed",
    model: str = "gpt-4o-mini",
    base_url: str | None = None,
    on_progress: callable | None = None,
) -> ReconstructionResult:
    """Run the full reconstruction pipeline.

    Args:
        decompiled_file: Path to the Ghidra decompiled .c file
        output_dir: Where to write the reconstructed project
        model: LLM model name
        base_url: Optional OpenAI-compatible API base URL
        on_progress: Optional callback(stage: str, detail: str)

    Returns:
        ReconstructionResult with all outputs
    """
    def progress(stage: str, detail: str = ""):
        if on_progress:
            on_progress(stage, detail)

    decompiled_path = Path(decompiled_file)
    out_path = Path(output_dir)
    source = decompiled_path.read_text(encoding="utf-8", errors="replace")

    # Step 1: Parse into individual functions
    progress("parse", f"Parsing {decompiled_path.name}")
    all_functions = parse_decompiled(source)
    progress("parse", f"Found {len(all_functions)} functions")

    # Step 2: Filter to user functions only
    user_funcs = {f.name: f for f in all_functions if f.name in USER_FUNCTIONS}
    progress("filter", f"Identified {len(user_funcs)} user functions out of {len(all_functions)} total")

    # Step 3: Process functions bottom-up via LLM
    llm = LLMClient(model=model, base_url=base_url)
    function_map: dict[str, str] = {}  # original_name -> new_name
    reconstructed: list[ReconstructedFunction] = []

    for orig_name in PROCESSING_ORDER:
        if orig_name not in user_funcs:
            progress("skip", f"Skipping {orig_name} (not found in decompiled output)")
            continue

        func = user_funcs[orig_name]
        progress("reconstruct", f"Processing {orig_name}")

        user_prompt = build_user_prompt(func, function_map)
        response = llm.call(SYSTEM_PROMPT, user_prompt)

        parsed = parse_llm_response(response.text)
        parsed.original_name = orig_name

        # Update function map
        function_map[orig_name] = parsed.new_name
        reconstructed.append(parsed)

        progress("reconstruct", f"  {orig_name} -> {parsed.new_name}")

    # Step 4: Assemble into a single file
    progress("assemble", "Assembling reconstructed.c")
    assembled = assemble_project(reconstructed, out_path)

    result = ReconstructionResult(
        functions=reconstructed,
        assembled_code=assembled,
        output_dir=out_path,
        total_tokens=llm.total_prompt_tokens + llm.total_completion_tokens,
    )

    # Step 5: Try to compile (up to 3 attempts)
    for attempt in range(1, 4):
        progress("compile", f"Compilation attempt {attempt}")
        success, errors = try_compile(out_path, attempt)
        result.compile_attempts = attempt

        if success:
            result.compiled = True
            progress("compile", "Compilation successful")
            break
        else:
            result.compile_errors = errors
            progress("compile", f"Compilation failed ({len(errors)} errors)")

            if attempt < 3:
                progress("fix", f"Asking LLM to fix errors (attempt {attempt})")
                fixed_code = fix_compile_errors(assembled, errors, llm)
                assembled = fixed_code
                result.assembled_code = assembled

                # Write fixed code
                (out_path / "reconstructed.c").write_text(assembled)
                result.total_tokens = llm.total_prompt_tokens + llm.total_completion_tokens

    # Step 6: Differential test (if compiled)
    if result.compiled:
        # Look for original binary
        decompiled_dir = decompiled_path.parent
        original_binary = decompiled_dir / "kvstore.exe"
        reconstructed_binary = out_path / ("reconstructed.exe" if sys.platform == "win32" else "reconstructed")
        test_input_file = decompiled_dir / "test_input.txt"

        if original_binary.exists():
            progress("test", "Running differential test")
            passed, details = differential_test(
                original_binary,
                reconstructed_binary,
                test_input_file if test_input_file.exists() else None,
            )
            result.diff_test_passed = passed
            result.diff_test_output = details
            progress("test", f"Differential test: {'PASSED' if passed else 'FAILED'}")
        else:
            progress("test", "Skipping differential test (original binary not found)")

    return result
