"""Functional equivalence verification for Synthex reconstructions.

Compiles reconstructed C code, runs test inputs through both the original
binary and the reconstruction, and diffs the outputs.
"""

from __future__ import annotations

import subprocess
import tempfile
from dataclasses import dataclass, field
from pathlib import Path


@dataclass
class VerifyResult:
    """Result of a functional equivalence test."""
    compiled: bool = False
    compile_errors: list[str] = field(default_factory=list)
    tests_run: int = 0
    tests_passed: int = 0
    tests_failed: int = 0
    failures: list[dict] = field(default_factory=list)

    @property
    def all_passed(self) -> bool:
        return self.compiled and self.tests_failed == 0 and self.tests_run > 0


def _compile(source_path: Path, output_path: Path) -> tuple[bool, list[str]]:
    """Compile a C file. Returns (success, errors)."""
    try:
        result = subprocess.run(
            ["gcc", "-O2", "-o", str(output_path), str(source_path)],
            capture_output=True, text=True, timeout=30,
        )
        if result.returncode == 0:
            return True, []
        errors = [l.strip() for l in result.stderr.splitlines() if "error:" in l][:10]
        return False, errors
    except FileNotFoundError:
        return False, ["gcc not found"]
    except subprocess.TimeoutExpired:
        return False, ["compilation timed out"]


def _run_with_input(binary: Path, input_text: str, timeout: int = 5) -> tuple[str, str, int]:
    """Run a binary with stdin input. Returns (stdout, stderr, returncode)."""
    try:
        result = subprocess.run(
            [str(binary)],
            input=input_text,
            capture_output=True, text=True,
            timeout=timeout,
        )
        return result.stdout, result.stderr, result.returncode
    except subprocess.TimeoutExpired:
        return "", "timeout", -1
    except Exception as e:
        return "", str(e), -1


DEFAULT_TEST_CASES = [
    {
        "name": "set and get",
        "input": "set hello world\nget hello\nquit\n",
    },
    {
        "name": "get missing key",
        "input": "get nonexistent\nquit\n",
    },
    {
        "name": "set, delete, get",
        "input": "set foo bar\ndel foo\nget foo\nquit\n",
    },
    {
        "name": "multiple operations",
        "input": "set a 1\nset b 2\nset c 3\nget a\nget b\nget c\ndel b\nget b\nquit\n",
    },
    {
        "name": "overwrite value",
        "input": "set key first\nget key\nset key second\nget key\nquit\n",
    },
]


def verify(
    original_binary: str | Path,
    reconstructed_source: str | Path,
    test_cases: list[dict] | None = None,
    original_source: str | Path | None = None,
) -> VerifyResult:
    """Verify functional equivalence between original binary and reconstructed source.

    If original_binary is a .c file (or original_source is provided),
    it will be compiled first.

    Args:
        original_binary: Path to the original binary (or .c source)
        reconstructed_source: Path to the reconstructed .c file
        test_cases: List of {"name": str, "input": str} test cases
        original_source: Optional .c source if original_binary needs compilation
    """
    result = VerifyResult()
    cases = test_cases or DEFAULT_TEST_CASES

    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = Path(tmpdir)
        orig_bin = tmp / "original"
        recon_bin = tmp / "reconstructed"

        # Handle original — binary or source
        orig_path = Path(original_binary)
        if original_source:
            ok, errors = _compile(Path(original_source), orig_bin)
            if not ok:
                result.compile_errors = [f"Original: {e}" for e in errors]
                return result
        elif orig_path.suffix == ".c":
            ok, errors = _compile(orig_path, orig_bin)
            if not ok:
                result.compile_errors = [f"Original: {e}" for e in errors]
                return result
        else:
            # Assume it's already a binary — copy it
            import shutil
            shutil.copy2(orig_path, orig_bin)
            orig_bin.chmod(0o755)

        # Compile reconstruction
        ok, errors = _compile(Path(reconstructed_source), recon_bin)
        if not ok:
            result.compile_errors = [f"Reconstructed: {e}" for e in errors]
            return result
        result.compiled = True

        # Run test cases
        for case in cases:
            result.tests_run += 1
            name = case["name"]
            input_text = case["input"]

            orig_out, orig_err, orig_rc = _run_with_input(orig_bin, input_text)
            recon_out, recon_err, recon_rc = _run_with_input(recon_bin, input_text)

            if orig_out == recon_out:
                result.tests_passed += 1
            else:
                result.tests_failed += 1
                result.failures.append({
                    "test": name,
                    "expected": orig_out,
                    "actual": recon_out,
                })

    return result
