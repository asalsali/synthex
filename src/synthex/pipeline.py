"""Pipeline orchestrator -- runs the multi-generational refinement loop."""

from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from synthex.agents.base import AgentResult, ExitReport
from synthex.agents.normalizer import Normalizer
from synthex.agents.optimizer import Optimizer
from synthex.agents.clarifier import Clarifier
from synthex.agents.synthesizer import Synthesizer
from synthex.llm import LLMClient
from synthex.scoring import ScoreBreakdown, score_code


@dataclass
class GenerationResult:
    """Result of a single generation in the pipeline."""

    name: str
    agent_type: str
    generation: int
    code: str
    scores: ScoreBreakdown
    exit_report: ExitReport
    tokens_used: int = 0


@dataclass
class PipelineResult:
    """Complete pipeline execution result."""

    input_code: str
    input_scores: ScoreBreakdown
    generations: list[GenerationResult] = field(default_factory=list)
    total_tokens: int = 0
    regression_detected: bool = False
    regression_details: list[str] = field(default_factory=list)

    @property
    def final_code(self) -> str:
        if self.generations:
            return self.generations[-1].code
        return self.input_code

    @property
    def final_scores(self) -> ScoreBreakdown:
        if self.generations:
            return self.generations[-1].scores
        return self.input_scores

    @property
    def score_progression(self) -> list[tuple[str, int]]:
        """Return list of (stage_name, score) tuples."""
        prog = [("Input", self.input_scores.total_score)]
        for gen in self.generations:
            prog.append((gen.name, gen.scores.total_score))
        return prog


def _check_regression(
    current_scores: ScoreBreakdown,
    parent_scores: ScoreBreakdown,
    stage_name: str,
) -> str | None:
    """Check if a generation regressed from its parent. Returns detail string or None."""
    if current_scores.total_score < parent_scores.total_score:
        delta = current_scores.total_score - parent_scores.total_score
        return (
            f"{stage_name}: score regressed {parent_scores.total_score} -> "
            f"{current_scores.total_score} ({delta})"
        )
    return None


def run_pipeline(
    input_code: str,
    model: str = "gpt-4o-mini",
    output_dir: str | Path = "synthex-output",
    generations: int = 3,
    base_url: str | None = None,
) -> PipelineResult:
    """Run the full multi-generational refinement pipeline.

    Pipeline: Input -> G1 Normalizer -> G2a Optimizer + G2b Clarifier -> G3 Synthesizer

    Args:
        input_code: Raw decompiled C source code.
        model: LLM model name (OpenAI-compatible).
        output_dir: Directory for output files and exit reports.
        generations: Number of generations to run (1-3).
        base_url: Optional OpenAI-compatible API base URL.

    Returns:
        PipelineResult with all generation outputs and scores.
    """
    output_path = Path(output_dir)
    output_path.mkdir(parents=True, exist_ok=True)
    inheritance_dir = output_path / "inheritance"
    inheritance_dir.mkdir(parents=True, exist_ok=True)

    llm = LLMClient(model=model, base_url=base_url)
    input_scores = score_code(input_code)

    result = PipelineResult(
        input_code=input_code,
        input_scores=input_scores,
    )

    # Save input
    (output_path / "g0-input.c").write_text(input_code)

    # --- G1: Normalizer ---
    normalizer = Normalizer(llm=llm, output_dir=inheritance_dir)
    g1_result = normalizer.run(input_code)

    g1_gen = GenerationResult(
        name="G1 Normalizer",
        agent_type="normalizer",
        generation=1,
        code=g1_result.code,
        scores=g1_result.scores,
        exit_report=g1_result.exit_report,
        tokens_used=g1_result.exit_report.tokens_consumed,
    )
    result.generations.append(g1_gen)
    result.total_tokens += g1_gen.tokens_used
    (output_path / "g1-normalizer.c").write_text(g1_result.code)

    # Check regression
    reg = _check_regression(g1_result.scores, input_scores, "G1 Normalizer")
    if reg:
        result.regression_detected = True
        result.regression_details.append(reg)

    if generations < 2:
        return result

    # --- G2a: Optimizer ---
    optimizer = Optimizer(llm=llm, output_dir=inheritance_dir)
    g2a_result = optimizer.run(g1_result.code, parent_report=g1_result.exit_report)

    g2a_gen = GenerationResult(
        name="G2a Optimizer",
        agent_type="optimizer",
        generation=2,
        code=g2a_result.code,
        scores=g2a_result.scores,
        exit_report=g2a_result.exit_report,
        tokens_used=g2a_result.exit_report.tokens_consumed,
    )
    result.generations.append(g2a_gen)
    result.total_tokens += g2a_gen.tokens_used
    (output_path / "g2a-optimizer.c").write_text(g2a_result.code)

    reg = _check_regression(g2a_result.scores, g1_result.scores, "G2a Optimizer")
    if reg:
        result.regression_detected = True
        result.regression_details.append(reg)

    # --- G2b: Clarifier ---
    clarifier = Clarifier(llm=llm, output_dir=inheritance_dir)
    g2b_result = clarifier.run(g1_result.code, parent_report=g1_result.exit_report)

    g2b_gen = GenerationResult(
        name="G2b Clarifier",
        agent_type="clarifier",
        generation=2,
        code=g2b_result.code,
        scores=g2b_result.scores,
        exit_report=g2b_result.exit_report,
        tokens_used=g2b_result.exit_report.tokens_consumed,
    )
    result.generations.append(g2b_gen)
    result.total_tokens += g2b_gen.tokens_used
    (output_path / "g2b-clarifier.c").write_text(g2b_result.code)

    reg = _check_regression(g2b_result.scores, g1_result.scores, "G2b Clarifier")
    if reg:
        result.regression_detected = True
        result.regression_details.append(reg)

    if generations < 3:
        return result

    # --- G3: Synthesizer ---
    synthesizer = Synthesizer(llm=llm, output_dir=inheritance_dir)
    g3_result = synthesizer.run(
        code=g1_result.code,  # baseline
        parent_reports=[g2a_result.exit_report, g2b_result.exit_report],
        optimizer_code=g2a_result.code,
        clarifier_code=g2b_result.code,
    )

    g3_gen = GenerationResult(
        name="G3 Synthesizer",
        agent_type="synthesizer",
        generation=3,
        code=g3_result.code,
        scores=g3_result.scores,
        exit_report=g3_result.exit_report,
        tokens_used=g3_result.exit_report.tokens_consumed,
    )
    result.generations.append(g3_gen)
    result.total_tokens += g3_gen.tokens_used
    (output_path / "g3-synthesizer.c").write_text(g3_result.code)

    # Check regression against best G2 parent
    best_g2_score = max(g2a_result.scores.total_score, g2b_result.scores.total_score)
    if g3_result.scores.total_score < best_g2_score:
        detail = (
            f"G3 Synthesizer: score ({g3_result.scores.total_score}) below "
            f"best G2 parent ({best_g2_score})"
        )
        result.regression_detected = True
        result.regression_details.append(detail)

    return result
