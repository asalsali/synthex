"""Tests for synthex.scoring — deterministic, no LLM."""

import pytest
from synthex.scoring import (
    score_code,
    ScoreBreakdown,
    _extract_identifiers,
    _is_meaningful_name,
    _count_gotos,
    _count_typed_declarations,
    _count_comment_lines,
    _cyclomatic_complexity,
)


class TestScoreBreakdown:
    def test_total_score_range(self, clean_c_code):
        breakdown = score_code(clean_c_code)
        assert 0 <= breakdown.total_score <= 100

    def test_clean_code_scores_higher(self, clean_c_code, ghidra_c_code):
        clean = score_code(clean_c_code)
        ghidra = score_code(ghidra_c_code)
        assert clean.total_score > ghidra.total_score

    def test_empty_code(self):
        breakdown = score_code("")
        assert breakdown.total_score >= 0
        assert breakdown.lines == 1

    def test_score_fields_populated(self, clean_c_code):
        b = score_code(clean_c_code)
        assert b.lines > 0
        assert isinstance(b.goto_count, int)
        assert isinstance(b.meaningful_names, float)
        assert isinstance(b.comment_density, float)
        assert isinstance(b.cyclomatic_complexity, int)


class TestGotos:
    def test_no_gotos(self, clean_c_code):
        assert _count_gotos(clean_c_code) == 0

    def test_has_gotos(self, ghidra_c_code):
        assert _count_gotos(ghidra_c_code) == 2

    def test_goto_in_label(self):
        code = "goto LAB_1; goto LAB_2; goto LAB_3;"
        assert _count_gotos(code) == 3


class TestIdentifiers:
    def test_extracts_identifiers(self):
        code = "int result = process_items(data, count);"
        ids = _extract_identifiers(code)
        assert "result" in ids
        assert "process_items" in ids

    def test_excludes_keywords(self):
        code = "int x = 0; return x;"
        ids = _extract_identifiers(code)
        assert "int" not in ids
        assert "return" not in ids

    def test_excludes_short_names(self):
        ids = _extract_identifiers("int x;")
        # Single char 'x' has len <= 1, excluded
        assert "x" not in ids


class TestMeaningfulNames:
    def test_meaningful_name(self):
        assert _is_meaningful_name("process_items") is True
        assert _is_meaningful_name("total_count") is True

    def test_generated_name(self):
        assert _is_meaningful_name("iVar1") is False
        assert _is_meaningful_name("local_28") is False
        assert _is_meaningful_name("param_1") is False
        assert _is_meaningful_name("FUN_00401000") is False
        assert _is_meaningful_name("DAT_00402000") is False
        assert _is_meaningful_name("undefined8") is False

    def test_short_name(self):
        assert _is_meaningful_name("ab") is False
        assert _is_meaningful_name("abc") is False

    def test_pvvar(self):
        assert _is_meaningful_name("pvVar4") is False
        assert _is_meaningful_name("uVar3") is False
        assert _is_meaningful_name("lVar2") is False


class TestTypedDeclarations:
    def test_typed(self):
        code = "int count = 0;\nchar *name = NULL;"
        typed, total = _count_typed_declarations(code)
        assert typed >= 1
        assert total >= 1

    def test_undefined_is_weak(self):
        code = "undefined8 uVar3;\nundefined4 local_28;"
        typed, total = _count_typed_declarations(code)
        assert typed == 0  # undefined types are weak
        assert total >= 1


class TestCommentLines:
    def test_line_comments(self):
        code = "// comment\nint x;\n// another"
        assert _count_comment_lines(code) == 2

    def test_block_comments(self):
        code = "/* start\n * middle\n */\nint x;"
        assert _count_comment_lines(code) == 3

    def test_no_comments(self):
        assert _count_comment_lines("int x = 0;") == 0

    def test_inline_block(self):
        code = "int x; /* inline comment */"
        assert _count_comment_lines(code) == 1


class TestCyclomaticComplexity:
    def test_simple_function(self):
        code = "int f() { return 1; }"
        assert _cyclomatic_complexity(code) == 1

    def test_if_else(self):
        code = "if (x) { a(); } else { b(); }"
        # base 1 + if + else = 3
        assert _cyclomatic_complexity(code) == 3

    def test_loop(self):
        code = "for (int i = 0; i < n; i++) { if (x) {} }"
        cc = _cyclomatic_complexity(code)
        assert cc >= 3  # base + for + if

    def test_ternary(self):
        code = "int x = a ? b : c;"
        cc = _cyclomatic_complexity(code)
        assert cc >= 2  # base + ?


class TestMagicNumbers:
    def test_magic_hex(self, ghidra_c_code):
        b = score_code(ghidra_c_code)
        assert b.magic_number_count > 0

    def test_no_magic(self, clean_c_code):
        b = score_code(clean_c_code)
        assert b.magic_number_count == 0


class TestSampleFile:
    def test_sample_scores_low(self, sample_code):
        b = score_code(sample_code)
        assert b.total_score < 40  # raw Ghidra output should score low
        assert b.goto_count > 0
