"""Tests for the G0.5 Type Recovery agent — static analysis only, no LLM."""

import pytest
from synthex.agents.type_recovery import (
    analyze_types,
    _find_alloc_sizes,
    _find_pointer_strides,
    _find_constants,
    _infer_struct_layout,
    _infer_defines,
    TypeRecoveryResult,
)


class TestAllocSizes:
    def test_calloc_hex(self):
        code = 'ptr = calloc(1, 0x208);'
        sizes = _find_alloc_sizes(code)
        assert any(s == 520 for s, _ in sizes)

    def test_malloc_decimal(self):
        code = 'ptr = malloc(648);'
        sizes = _find_alloc_sizes(code)
        assert any(s == 648 for s, _ in sizes)

    def test_no_alloc(self):
        code = 'int x = 5;'
        assert _find_alloc_sizes(code) == []


class TestPointerStrides:
    def test_offset_pattern(self):
        code = '*(char **)(ptr + 0x280)'
        strides = _find_pointer_strides(code)
        assert any(s == 640 for s, _ in strides)

    def test_small_offset_ignored(self):
        code = 'ptr + 0x2'
        strides = _find_pointer_strides(code)
        # Offset 2 is too small to be a struct field
        assert not any(s == 2 for s, _ in strides)


class TestConstants:
    def test_bitmask(self):
        code = 'hash & 0x3f'
        consts = _find_constants(code)
        assert any(v == 0x3f for v, _ in consts)

    def test_comparison(self):
        code = 'if (strlen(key) < 128)'
        consts = _find_constants(code)
        assert any(v == 128 for v, _ in consts)

    def test_djb2(self):
        code = 'hash = hash * 33 + c;'
        consts = _find_constants(code)
        assert any(v == 33 for v, _ in consts)


class TestStructInference:
    def test_kvstore_from_520(self):
        hints = _infer_struct_layout(520, [128, 640])
        names = [h.name for h in hints]
        assert "KVStore" in names

    def test_entry_from_648(self):
        hints = _infer_struct_layout(648, [128, 640])
        names = [h.name for h in hints]
        assert "Entry" in names

    def test_unknown_size(self):
        hints = _infer_struct_layout(999, [])
        assert len(hints) == 0


class TestDefineInference:
    def test_table_size(self):
        consts = [(0x3f, "bitmask: & 0x3f")]
        hints = _infer_defines(consts)
        names = [h.name for h in hints]
        assert "TABLE_SIZE" in names

    def test_max_key_len(self):
        consts = [(128, "comparison: < 128")]
        hints = _infer_defines(consts)
        names = [h.name for h in hints]
        assert "MAX_KEY_LEN" in names


class TestFullAnalysis:
    def test_real_ghidra_output(self, sample_code):
        result = analyze_types(sample_code)
        assert isinstance(result, TypeRecoveryResult)
        assert len(result.alloc_sizes) > 0

    def test_header_generated(self, sample_code):
        result = analyze_types(sample_code)
        assert "TypeRecovery" in result.header or len(result.header) > 0

    def test_clean_code_no_hints(self, clean_c_code):
        result = analyze_types(clean_c_code)
        # Clean code shouldn't have Ghidra-style allocation patterns
        assert len(result.alloc_sizes) == 0
