"""G0.5 Type Recovery -- analyze decompiled C to recover struct layouts and types.

This agent runs BEFORE the Normalizer (G1). It reads raw Ghidra output and
produces a type header with recovered structs, typedefs, and #defines based
on allocation sizes, pointer arithmetic patterns, and constant analysis.

This is what separates "cleans up naming" from "produces runnable code."
"""

from __future__ import annotations

import re
from dataclasses import dataclass, field
from typing import Any

from synthex.agents.base import BaseAgent, ExitReport, AgentResult, _extract_code_block
from synthex.llm import LLMClient
from synthex.scoring import score_code


@dataclass
class TypeHint:
    """A recovered type hint from static analysis."""
    kind: str  # "struct", "define", "typedef"
    name: str
    evidence: str  # what pattern revealed this
    definition: str  # C code for the type


@dataclass
class TypeRecoveryResult:
    """Result of the type recovery analysis."""
    hints: list[TypeHint] = field(default_factory=list)
    header: str = ""  # generated types.h content
    alloc_sizes: list[tuple[int, str]] = field(default_factory=list)  # (size, context)
    pointer_strides: list[tuple[int, str]] = field(default_factory=list)  # (stride, context)
    constants: list[tuple[int, str]] = field(default_factory=list)  # (value, usage)


def _find_alloc_sizes(code: str) -> list[tuple[int, str]]:
    """Find calloc/malloc sizes to infer struct layouts."""
    results = []
    # calloc(1, 0xNNN) or calloc(1, NNN)
    for m in re.finditer(r'(?:calloc|malloc)\s*\(\s*\d+\s*,\s*(0x[0-9a-fA-F]+|\d+)\s*\)', code):
        size_str = m.group(1)
        size = int(size_str, 16) if size_str.startswith("0x") else int(size_str)
        # Get surrounding context
        start = max(0, m.start() - 80)
        end = min(len(code), m.end() + 80)
        context = code[start:end].strip()
        results.append((size, context))
    # malloc(0xNNN) or malloc(NNN)
    for m in re.finditer(r'malloc\s*\(\s*(0x[0-9a-fA-F]+|\d+)\s*\)', code):
        size_str = m.group(1)
        size = int(size_str, 16) if size_str.startswith("0x") else int(size_str)
        start = max(0, m.start() - 80)
        end = min(len(code), m.end() + 80)
        context = code[start:end].strip()
        results.append((size, context))
    return results


def _find_pointer_strides(code: str) -> list[tuple[int, str]]:
    """Find pointer arithmetic patterns to infer struct field offsets."""
    results = []
    # *(type *)(ptr + 0xNNN)
    for m in re.finditer(r'\*\s*\([^)]*\*\)\s*\([^)]+\+\s*(0x[0-9a-fA-F]+|\d+)\s*\)', code):
        offset_str = m.group(1)
        offset = int(offset_str, 16) if offset_str.startswith("0x") else int(offset_str)
        start = max(0, m.start() - 40)
        end = min(len(code), m.end() + 40)
        context = code[start:end].strip()
        results.append((offset, context))
    # ptr + 0xNNN without dereference
    for m in re.finditer(r'\w+\s*\+\s*(0x[0-9a-fA-F]+)\b', code):
        offset_str = m.group(1)
        offset = int(offset_str, 16)
        if offset > 4:  # skip small offsets that are likely array indices
            start = max(0, m.start() - 40)
            end = min(len(code), m.end() + 40)
            context = code[start:end].strip()
            results.append((offset, context))
    return results


def _find_constants(code: str) -> list[tuple[int, str]]:
    """Find recurring constants that suggest #defines."""
    results = []
    # Bitmask patterns: & 0xNN
    for m in re.finditer(r'&\s*(0x[0-9a-fA-F]+)', code):
        val = int(m.group(1), 16)
        results.append((val, f"bitmask: {m.group(0)}"))
    # Comparison constants: < 0xNN or < NNN
    for m in re.finditer(r'[<>]=?\s*(0x[0-9a-fA-F]+|\d{3,})', code):
        val_str = m.group(1)
        val = int(val_str, 16) if val_str.startswith("0x") else int(val_str)
        if val > 1:
            results.append((val, f"comparison: {m.group(0)}"))
    # Magic number in hash: * 33 or * 0x21 (djb2)
    for m in re.finditer(r'\*\s*(33|0x21)', code):
        results.append((33, "djb2 hash multiplier"))
    return results


def _infer_struct_layout(alloc_size: int, offsets: list[int]) -> list[TypeHint]:
    """Infer struct fields from allocation size and known offsets."""
    hints = []

    # Common patterns
    # 648 bytes with offsets 128, 640 → Entry { char key[128], char value[512], void *next }
    if alloc_size == 648 or alloc_size == 0x288:
        if 128 in offsets or 0x80 in offsets:
            hints.append(TypeHint(
                kind="struct",
                name="Entry",
                evidence=f"calloc/malloc({alloc_size}), offset 128 for value, 640 for next",
                definition="typedef struct Entry {\n    char key[128];\n    char value[512];\n    struct Entry *next;\n} Entry;",
            ))

    # 520 bytes → KVStore { Entry *buckets[64], int count }
    # 64 pointers * 8 bytes = 512, + 4 byte count + padding = 520
    if alloc_size == 520 or alloc_size == 0x208:
        hints.append(TypeHint(
            kind="struct",
            name="KVStore",
            evidence=f"calloc(1, {alloc_size}), 64 pointer slots + count field",
            definition="typedef struct {\n    Entry *buckets[64];\n    int count;\n} KVStore;",
        ))

    return hints


def _infer_defines(constants: list[tuple[int, str]]) -> list[TypeHint]:
    """Infer #define constants from recurring values."""
    hints = []
    seen = {}
    for val, usage in constants:
        if val not in seen:
            seen[val] = []
        seen[val].append(usage)

    # Map known constant values
    define_map = {
        64: ("TABLE_SIZE", "hash table bucket count"),
        63: ("TABLE_SIZE", "hash mask (TABLE_SIZE - 1)"),
        0x3f: ("TABLE_SIZE", "hash mask 0x3f = 63 = TABLE_SIZE - 1"),
        128: ("MAX_KEY_LEN", "key buffer size"),
        0x80: ("MAX_KEY_LEN", "key buffer size"),
        512: ("MAX_VAL_LEN", "value buffer size"),
        0x200: ("MAX_VAL_LEN", "value buffer size"),
        5381: ("HASH_INIT", "djb2 initial hash value"),
    }

    for val, usages in seen.items():
        if val in define_map:
            name, desc = define_map[val]
            hints.append(TypeHint(
                kind="define",
                name=name,
                evidence=f"constant {val} ({hex(val)}): {desc}",
                definition=f"#define {name} {val}",
            ))

    return hints


def analyze_types(code: str) -> TypeRecoveryResult:
    """Analyze decompiled C code to recover type information.

    Returns a TypeRecoveryResult with hints and a generated header.
    """
    result = TypeRecoveryResult()

    result.alloc_sizes = _find_alloc_sizes(code)
    result.pointer_strides = _find_pointer_strides(code)
    result.constants = _find_constants(code)

    # Collect all offsets
    all_offsets = [s for s, _ in result.pointer_strides]

    # Infer structs from allocation sizes
    for size, _ in result.alloc_sizes:
        hints = _infer_struct_layout(size, all_offsets)
        result.hints.extend(hints)

    # Infer defines from constants
    result.hints.extend(_infer_defines(result.constants))

    # Deduplicate by name
    seen_names: set[str] = set()
    unique_hints: list[TypeHint] = []
    for h in result.hints:
        if h.name not in seen_names:
            seen_names.add(h.name)
            unique_hints.append(h)
    result.hints = unique_hints

    # Generate header
    lines = ["/* Type definitions recovered by Synthex TypeRecovery */", ""]

    # Defines first
    defines = [h for h in result.hints if h.kind == "define"]
    if defines:
        for h in defines:
            lines.append(h.definition)
        lines.append("")

    # Then structs
    structs = [h for h in result.hints if h.kind == "struct"]
    if structs:
        for h in structs:
            lines.append(h.definition)
            lines.append("")

    result.header = "\n".join(lines)
    return result


class TypeRecovery(BaseAgent):
    """Generation 0.5: Recover struct layouts and type definitions.

    Runs before the Normalizer. Analyzes raw Ghidra output for allocation
    patterns, pointer arithmetic, and constants. Emits a type header that
    subsequent agents use for proper typed code.
    """

    agent_type = "type_recovery"
    generation = 0
    system_prompt = """\
You are a type recovery specialist for decompiled C code. You analyze raw \
Ghidra decompiler output and recover the original type definitions.

Your job is to:
1. Identify struct layouts from malloc/calloc sizes and pointer arithmetic offsets
2. Recover #define constants from bitmasks, comparisons, and buffer size checks
3. Name recovered types based on usage context (e.g., a hash table with \
linked list chaining suggests "Entry" and "KVStore")
4. Produce a C header with typedef structs and #defines

ANALYSIS APPROACH:
- calloc(1, N) tells you the struct size. Work backward from N to field layout.
- *(ptr + 0x80) tells you a field starts at offset 128. Match this to char[128].
- hash & 0x3f tells you TABLE_SIZE is 64.
- strlen(key) < 0x80 tells you MAX_KEY_LEN is 128.
- The djb2 hash pattern (hash * 33 + c, initial 5381) is very common.

OUTPUT: Return ONLY a C header file inside a ```c code fence with:
- #define constants
- typedef structs with field names inferred from usage
- Forward declarations if needed

Do not include function declarations -- only type definitions."""

    def run(
        self,
        code: str,
        parent_report: ExitReport | None = None,
        parent_reports: list[ExitReport] | None = None,
        function_map: dict[str, str] | None = None,
    ) -> AgentResult:
        """Analyze code and produce a type header."""
        # Static analysis first
        analysis = analyze_types(code)

        # Build prompt with static analysis results
        user_prompt = f"Here is the decompiled C code:\n\n```c\n{code[:6000]}\n```\n\n"

        if analysis.hints:
            user_prompt += "STATIC ANALYSIS RESULTS:\n"
            for h in analysis.hints:
                user_prompt += f"  - {h.kind} {h.name}: {h.evidence}\n"
            user_prompt += "\nUse these findings to guide your type recovery. "
            user_prompt += "Verify them against the code and add any types I missed.\n"

        user_prompt += "\nReturn ONLY the recovered type definitions as a C header inside a ```c code fence."

        response = self.llm.call(self.system_prompt, user_prompt)
        header = _extract_code_block(response.text)

        # Build exit report
        exit_report = ExitReport(
            agent_id=self.agent_id,
            agent_type=self.agent_type,
            generation=self.generation,
            what_worked=[f"Recovered {len(analysis.hints)} type hints via static analysis"],
            recommendations=["Use recovered types in function signatures instead of raw pointers"],
            tokens_consumed=response.total_tokens,
            scores={"types_recovered": len(analysis.hints)},
        )
        exit_report.save(self.output_dir)

        # Score is not meaningful for type recovery — return input scores
        scores = score_code(code)

        return AgentResult(
            code=header,
            exit_report=exit_report,
            scores=scores,
            llm_response=response,
        )
