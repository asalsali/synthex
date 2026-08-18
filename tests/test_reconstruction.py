"""Tests for the kvstore reconstruction — v10.

Verifies the reconstructed C code scores correctly and matches
the original source structure. Compilation and functional equivalence
tests require gcc and are skipped if unavailable.
"""

import subprocess
import tempfile
from pathlib import Path

import pytest
from synthex.scoring import score_code


REAL_DIR = Path(__file__).resolve().parent.parent / "examples" / "real"
RECON_DIR = Path(__file__).resolve().parent.parent / "reconstructed-v10"
ORIGINAL = REAL_DIR / "source.c"
GHIDRA = REAL_DIR / "decompiled_output.c"
RECONSTRUCTED = RECON_DIR / "reconstructed.c"


def _has_gcc() -> bool:
    try:
        subprocess.run(["gcc", "--version"], capture_output=True, timeout=5)
        return True
    except (FileNotFoundError, subprocess.TimeoutExpired):
        return False


class TestReconstructionScoring:
    def test_reconstructed_exists(self):
        assert RECONSTRUCTED.exists()

    def test_scores_higher_than_ghidra(self):
        ghidra = score_code(GHIDRA.read_text(encoding="utf-8"))
        recon = score_code(RECONSTRUCTED.read_text(encoding="utf-8"))
        assert recon.total_score > ghidra.total_score

    def test_matches_original_score(self):
        original = score_code(ORIGINAL.read_text(encoding="utf-8"))
        recon = score_code(RECONSTRUCTED.read_text(encoding="utf-8"))
        # Should be within 5 points of original
        assert abs(recon.total_score - original.total_score) <= 5

    def test_no_gotos(self):
        recon = score_code(RECONSTRUCTED.read_text(encoding="utf-8"))
        assert recon.goto_count == 0

    def test_no_ghidra_artifacts_in_code(self):
        code = RECONSTRUCTED.read_text(encoding="utf-8")
        lines = code.splitlines()
        # Only check non-comment lines
        code_lines = [l for l in lines if not l.strip().startswith("/*")
                      and not l.strip().startswith("*") and not l.strip().startswith("//")]
        code_only = "\n".join(code_lines)
        artifacts = ["undefined8", "undefined4", "iVar", "uVar", "pvVar",
                     "local_", "LAB_", "__acrt_iob_func", "0x746573"]
        for artifact in artifacts:
            assert artifact not in code_only, f"Ghidra artifact in code: {artifact}"

    def test_all_functions_present(self):
        code = RECONSTRUCTED.read_text(encoding="utf-8")
        expected = ["hash_key", "kv_create", "kv_set", "kv_get",
                    "kv_delete", "kv_save", "kv_load", "kv_destroy", "main"]
        for fn in expected:
            assert f"{fn}(" in code, f"Missing function: {fn}"

    def test_proper_types_used(self):
        code = RECONSTRUCTED.read_text(encoding="utf-8")
        assert "KVStore *" in code, "Should use KVStore * not raw pointers"
        assert "Entry *" in code, "Should use Entry * struct type"
        assert "long long *store" not in code, "Should not use raw long long *"


@pytest.mark.skipif(not _has_gcc(), reason="gcc not available")
class TestReconstructionCompiles:
    def test_compiles(self):
        result = subprocess.run(
            ["gcc", "-O2", "-o", "/dev/null", str(RECONSTRUCTED)],
            capture_output=True, text=True, timeout=10,
        )
        assert result.returncode == 0, f"Compile failed:\n{result.stderr}"

    def test_functional_equivalence(self):
        """Compile both, run same commands, compare output."""
        with tempfile.TemporaryDirectory() as tmpdir:
            orig_bin = Path(tmpdir) / "original"
            recon_bin = Path(tmpdir) / "reconstructed"

            # Compile both
            subprocess.run(
                ["gcc", "-O2", "-o", str(orig_bin), str(ORIGINAL)],
                check=True, timeout=10,
            )
            subprocess.run(
                ["gcc", "-O2", "-o", str(recon_bin), str(RECONSTRUCTED)],
                check=True, timeout=10,
            )

            # Run same commands on both
            commands = "set foo bar\nget foo\nset baz qux\nget baz\nget missing\ndel foo\nget foo\nquit\n"

            orig_result = subprocess.run(
                [str(orig_bin)], input=commands,
                capture_output=True, text=True, timeout=5,
            )
            recon_result = subprocess.run(
                [str(recon_bin)], input=commands,
                capture_output=True, text=True, timeout=5,
            )

            assert orig_result.stdout == recon_result.stdout, (
                f"Output differs:\nOriginal:\n{orig_result.stdout}\n"
                f"Reconstructed:\n{recon_result.stdout}"
            )
