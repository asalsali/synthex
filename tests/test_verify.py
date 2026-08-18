"""Tests for synthex.verify — data structures only, compile tests need gcc."""

import subprocess
from pathlib import Path
from unittest.mock import patch

import pytest
from synthex.verify import VerifyResult, DEFAULT_TEST_CASES


class TestVerifyResult:
    def test_all_passed(self):
        r = VerifyResult(compiled=True, tests_run=3, tests_passed=3, tests_failed=0)
        assert r.all_passed

    def test_not_passed_on_failure(self):
        r = VerifyResult(compiled=True, tests_run=3, tests_passed=2, tests_failed=1)
        assert not r.all_passed

    def test_not_passed_no_compile(self):
        r = VerifyResult(compiled=False, tests_run=0, tests_passed=0, tests_failed=0)
        assert not r.all_passed

    def test_not_passed_no_tests(self):
        r = VerifyResult(compiled=True, tests_run=0, tests_passed=0, tests_failed=0)
        assert not r.all_passed


class TestDefaultCases:
    def test_has_cases(self):
        assert len(DEFAULT_TEST_CASES) >= 3

    def test_cases_have_fields(self):
        for case in DEFAULT_TEST_CASES:
            assert "name" in case
            assert "input" in case
            assert case["input"].endswith("quit\n")
