"""Tests for synthex agents — exit reports, code extraction, no LLM calls."""

import json
from pathlib import Path
from unittest.mock import MagicMock, patch

import pytest
from synthex.agents.base import (
    BaseAgent,
    ExitReport,
    AgentResult,
    _extract_code_block,
)
from synthex.agents.normalizer import Normalizer
from synthex.agents.optimizer import Optimizer
from synthex.agents.clarifier import Clarifier
from synthex.agents.synthesizer import Synthesizer
from synthex.llm import LLMClient, LLMResponse


class TestExitReport:
    def test_to_dict(self):
        report = ExitReport(
            agent_id="test-1",
            agent_type="normalizer",
            generation=1,
            what_worked=["Removed gotos"],
            what_failed=["Naming incomplete"],
            recommendations=["Focus on naming"],
        )
        d = report.to_dict()
        assert d["agentId"] == "test-1"
        assert d["agentType"] == "normalizer"
        assert d["generation"] == 1
        assert "Removed gotos" in d["whatWorked"]

    def test_to_prompt_context(self):
        report = ExitReport(
            agent_id="test-1",
            agent_type="normalizer",
            generation=1,
            what_worked=["Improved naming"],
            gaps=[{"domain": "control_flow", "description": "3 gotos remain"}],
        )
        ctx = report.to_prompt_context()
        assert "normalizer" in ctx
        assert "Improved naming" in ctx
        assert "control_flow" in ctx

    def test_save(self, tmp_path):
        report = ExitReport(
            agent_id="test-save",
            agent_type="normalizer",
            generation=1,
        )
        path = report.save(tmp_path)
        assert path.exists()
        data = json.loads(path.read_text())
        assert data["agentId"] == "test-save"

    def test_emergent_skill_fields(self):
        report = ExitReport(
            agent_id="synth-1",
            agent_type="synthesizer",
            generation=3,
            emergent_skill="context-aware synthesis",
            emergent_skill_exercised=True,
        )
        d = report.to_dict()
        assert d["emergentSkill"] == "context-aware synthesis"
        assert d["emergentSkillExercised"] is True


class TestExtractCodeBlock:
    def test_fenced_c(self):
        text = "Some explanation\n```c\nint x = 1;\n```\nMore text"
        assert _extract_code_block(text) == "int x = 1;"

    def test_fenced_no_lang(self):
        text = "```\nint x = 1;\n```"
        assert _extract_code_block(text) == "int x = 1;"

    def test_no_fence_c_pattern(self):
        text = "Here is the code:\n#include <stdio.h>\nint main() {}\n"
        result = _extract_code_block(text)
        assert "#include <stdio.h>" in result

    def test_fallback_returns_all(self):
        text = "no code here at all"
        assert _extract_code_block(text) == "no code here at all"


class TestAgentTypes:
    def test_normalizer_metadata(self):
        assert Normalizer.agent_type == "normalizer"
        assert Normalizer.generation == 1

    def test_optimizer_metadata(self):
        assert Optimizer.agent_type == "optimizer"
        assert Optimizer.generation == 2

    def test_clarifier_metadata(self):
        assert Clarifier.agent_type == "clarifier"
        assert Clarifier.generation == 2

    def test_synthesizer_metadata(self):
        assert Synthesizer.agent_type == "synthesizer"
        assert Synthesizer.generation == 3


class TestAgentRun:
    def _make_mock_llm(self, response_code: str) -> LLMClient:
        """Create a mock LLM that returns the given code in a fence."""
        mock = MagicMock(spec=LLMClient)
        mock.call.return_value = LLMResponse(
            text=f"```c\n{response_code}\n```",
            prompt_tokens=100,
            completion_tokens=200,
            model="mock",
        )
        return mock

    def test_normalizer_run(self, tmp_path):
        refined = """\
#include <stdlib.h>

int process_data(long data, int count) {
    if (count < 1) {
        return -1;
    }
    void *buffer = malloc(24);
    if (buffer == NULL) {
        return -1;
    }
    return 0;
}
"""
        mock_llm = self._make_mock_llm(refined)
        agent = Normalizer(llm=mock_llm, output_dir=tmp_path)
        result = agent.run("int FUN_001() { goto LAB; LAB: return 0; }")

        assert isinstance(result, AgentResult)
        assert isinstance(result.exit_report, ExitReport)
        assert result.exit_report.agent_type == "normalizer"
        assert result.exit_report.tokens_consumed > 0
        mock_llm.call.assert_called()

    def test_exit_report_detects_goto_removal(self, tmp_path):
        input_code = "void f() { goto LAB; goto LAB2; LAB: LAB2: return; }"
        output_code = "void f() { return; }"
        mock_llm = self._make_mock_llm(output_code)
        agent = Normalizer(llm=mock_llm, output_dir=tmp_path)
        result = agent.run(input_code)
        assert any("goto" in w.lower() for w in result.exit_report.what_worked)
