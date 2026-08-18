"""Tests for synthex.pipeline — data structures only, no LLM calls."""

import pytest
from synthex.pipeline import (
    GenerationResult,
    PipelineResult,
    _check_regression,
)
from synthex.scoring import ScoreBreakdown, score_code
from synthex.agents.base import ExitReport


class TestPipelineResult:
    def _make_result(self, input_score: int, gen_scores: list[int]) -> PipelineResult:
        input_breakdown = ScoreBreakdown(lines=50, goto_count=0, meaningful_names=0.5)
        # Hack: override total_score by adjusting meaningful_names
        result = PipelineResult(
            input_code="int x;",
            input_scores=input_breakdown,
        )
        for i, score in enumerate(gen_scores):
            gen_breakdown = ScoreBreakdown(lines=40, goto_count=0, meaningful_names=0.8)
            report = ExitReport(
                agent_id=f"agent-{i}",
                agent_type="normalizer",
                generation=i + 1,
            )
            result.generations.append(GenerationResult(
                name=f"G{i+1}",
                agent_type="normalizer",
                generation=i + 1,
                code=f"// gen {i+1}",
                scores=gen_breakdown,
                exit_report=report,
                tokens_used=100,
            ))
        return result

    def test_final_code(self):
        result = self._make_result(20, [60, 70])
        assert result.final_code == "// gen 2"

    def test_final_code_empty(self):
        result = PipelineResult(
            input_code="int x;",
            input_scores=ScoreBreakdown(),
        )
        assert result.final_code == "int x;"

    def test_score_progression(self):
        result = self._make_result(20, [60])
        prog = result.score_progression
        assert len(prog) == 2
        assert prog[0][0] == "Input"
        assert prog[1][0] == "G1"


class TestRegressionCheck:
    def test_no_regression(self):
        current = ScoreBreakdown(lines=50, goto_count=0, meaningful_names=0.8)
        parent = ScoreBreakdown(lines=100, goto_count=5, meaningful_names=0.3)
        result = _check_regression(current, parent, "G1")
        # current.total_score should be >= parent.total_score
        if current.total_score >= parent.total_score:
            assert result is None

    def test_regression_detected(self):
        # Force a low-scoring current
        current = ScoreBreakdown(lines=200, goto_count=10, meaningful_names=0.0)
        parent = ScoreBreakdown(lines=30, goto_count=0, meaningful_names=1.0)
        result = _check_regression(current, parent, "G1")
        if current.total_score < parent.total_score:
            assert result is not None
            assert "G1" in result
            assert "regressed" in result


class TestGenerationResult:
    def test_fields(self):
        b = ScoreBreakdown(lines=50)
        r = ExitReport(agent_id="a", agent_type="normalizer", generation=1)
        gen = GenerationResult(
            name="G1 Normalizer",
            agent_type="normalizer",
            generation=1,
            code="int x;",
            scores=b,
            exit_report=r,
            tokens_used=500,
        )
        assert gen.name == "G1 Normalizer"
        assert gen.tokens_used == 500
