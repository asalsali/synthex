"""Base agent for multi-generational code refinement."""

from __future__ import annotations

import json
import re
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from synthex.llm import LLMClient, LLMResponse
from synthex.scoring import ScoreBreakdown, score_code


@dataclass
class ExitReport:
    """Structured exit report following Covenant inheritance pattern."""

    agent_id: str
    agent_type: str
    generation: int
    what_worked: list[str] = field(default_factory=list)
    what_failed: list[str] = field(default_factory=list)
    recommendations: list[str] = field(default_factory=list)
    gaps: list[dict[str, str]] = field(default_factory=list)
    scores: dict[str, Any] = field(default_factory=dict)
    emergent_skill: str | None = None
    emergent_skill_exercised: bool = False
    tokens_consumed: int = 0

    def to_dict(self) -> dict[str, Any]:
        return {
            "agentId": self.agent_id,
            "agentType": self.agent_type,
            "generation": self.generation,
            "whatWorked": self.what_worked,
            "whatFailed": self.what_failed,
            "recommendations": self.recommendations,
            "gaps": self.gaps,
            "scores": self.scores,
            "emergentSkill": self.emergent_skill,
            "emergentSkillExercised": self.emergent_skill_exercised,
            "tokensConsumed": self.tokens_consumed,
        }

    def to_prompt_context(self) -> str:
        """Distill exit report into prompt-ready context for the next agent."""
        lines = [f"=== Exit Report from {self.agent_type} (G{self.generation}) ==="]
        if self.what_worked:
            lines.append("What worked: " + "; ".join(self.what_worked))
        if self.what_failed:
            lines.append("What failed: " + "; ".join(self.what_failed))
        if self.recommendations:
            lines.append("Recommendations for you: " + "; ".join(self.recommendations))
        if self.gaps:
            gap_strs = [f"{g['domain']}: {g['description']}" for g in self.gaps]
            lines.append("Known gaps: " + "; ".join(gap_strs))
        return "\n".join(lines)

    def save(self, output_dir: Path) -> Path:
        """Write exit report to disk."""
        output_dir.mkdir(parents=True, exist_ok=True)
        path = output_dir / f"{self.agent_id}-exit_report.json"
        path.write_text(json.dumps(self.to_dict(), indent=2))
        return path


@dataclass
class AgentResult:
    """Result of a single agent run."""

    code: str
    exit_report: ExitReport
    scores: ScoreBreakdown
    llm_response: LLMResponse | None = None


def _extract_code_block(text: str) -> str:
    """Extract C code from LLM response, handling markdown fences."""
    # Try to find fenced code block
    match = re.search(r"```(?:c|cpp)?\s*\n(.*?)```", text, re.DOTALL)
    if match:
        return match.group(1).strip()

    # If no fences, look for code that starts with typical C patterns
    lines = text.strip().splitlines()
    code_lines: list[str] = []
    in_code = False
    for line in lines:
        stripped = line.strip()
        if not in_code:
            # Start collecting at first C-like line
            if (stripped.startswith("#include") or
                stripped.startswith("/*") or
                stripped.startswith("//") or
                stripped.startswith("typedef") or
                stripped.startswith("struct") or
                re.match(r"^(?:void|int|char|long|unsigned|static|const)\s", stripped)):
                in_code = True
                code_lines.append(line)
        else:
            code_lines.append(line)

    if code_lines:
        return "\n".join(code_lines)

    # Last resort: return everything
    return text.strip()


class BaseAgent:
    """Base class for all Synthex refinement agents."""

    agent_type: str = "base"
    generation: int = 0
    system_prompt: str = "You are a code refinement agent."

    def __init__(self, llm: LLMClient, output_dir: Path | None = None):
        self.llm = llm
        self.output_dir = output_dir or Path("memory/inheritance")
        self.agent_id = f"{self.agent_type}-g{self.generation}-{int(time.time())}"

    def _build_user_prompt(
        self,
        code: str,
        parent_report: ExitReport | None = None,
        parent_reports: list[ExitReport] | None = None,
    ) -> str:
        """Build the user prompt from code and parent context."""
        parts = ["Here is the C code to refine:\n"]
        parts.append(f"```c\n{code}\n```\n")

        if parent_reports:
            for report in parent_reports:
                parts.append(report.to_prompt_context())
                parts.append("")
        elif parent_report:
            parts.append(parent_report.to_prompt_context())

        parts.append(
            "\nReturn ONLY the refined C code inside a ```c code fence. "
            "Do not include explanations outside the code fence."
        )
        return "\n".join(parts)

    def _parse_exit_report_from_output(
        self,
        input_code: str,
        output_code: str,
        parent_report: ExitReport | None = None,
    ) -> ExitReport:
        """Build an exit report by comparing input and output."""
        input_scores = score_code(input_code)
        output_scores = score_code(output_code)

        what_worked: list[str] = []
        what_failed: list[str] = []
        recommendations: list[str] = []
        gaps: list[dict[str, str]] = []

        # Analyze improvements
        if output_scores.goto_count < input_scores.goto_count:
            removed = input_scores.goto_count - output_scores.goto_count
            what_worked.append(f"Removed {removed} goto statement(s)")
        elif output_scores.goto_count > 0:
            what_failed.append(f"{output_scores.goto_count} goto(s) remain")
            recommendations.append("Remaining gotos may require manual control flow recovery")

        if output_scores.meaningful_names > input_scores.meaningful_names:
            what_worked.append("Improved variable naming quality")
        else:
            what_failed.append("Variable naming did not improve significantly")
            recommendations.append("Focus on renaming short/generated identifiers")

        if output_scores.type_coverage > input_scores.type_coverage:
            what_worked.append("Improved type coverage")

        if output_scores.comment_density > input_scores.comment_density:
            what_worked.append("Added documentation comments")

        if output_scores.cyclomatic_complexity < input_scores.cyclomatic_complexity:
            what_worked.append("Reduced cyclomatic complexity")

        # Score regressions
        if output_scores.total_score < input_scores.total_score:
            what_failed.append(
                f"Overall score regressed: {input_scores.total_score} -> {output_scores.total_score}"
            )

        if output_scores.goto_count > 0:
            gaps.append({
                "domain": "control_flow",
                "description": f"{output_scores.goto_count} goto statements still present",
                "impact": "medium",
                "suggestedAction": "Convert remaining gotos to structured control flow",
            })

        if output_scores.meaningful_names < 0.8:
            gaps.append({
                "domain": "naming",
                "description": f"Only {output_scores.meaningful_names:.0%} of identifiers are meaningful",
                "impact": "medium",
                "suggestedAction": "Analyze usage patterns to infer better names",
            })

        return ExitReport(
            agent_id=self.agent_id,
            agent_type=self.agent_type,
            generation=self.generation,
            what_worked=what_worked,
            what_failed=what_failed,
            recommendations=recommendations,
            gaps=gaps,
            scores={
                "input": input_scores.total_score,
                "output": output_scores.total_score,
                "delta": output_scores.total_score - input_scores.total_score,
            },
        )

    def run(
        self,
        code: str,
        parent_report: ExitReport | None = None,
        parent_reports: list[ExitReport] | None = None,
    ) -> AgentResult:
        """Execute this agent: send code to LLM, score result, write exit report."""
        user_prompt = self._build_user_prompt(code, parent_report, parent_reports)
        response = self.llm.call(self.system_prompt, user_prompt)

        refined_code = _extract_code_block(response.text)
        output_scores = score_code(refined_code)

        exit_report = self._parse_exit_report_from_output(
            code, refined_code, parent_report
        )
        exit_report.tokens_consumed = response.total_tokens
        exit_report.save(self.output_dir)

        return AgentResult(
            code=refined_code,
            exit_report=exit_report,
            scores=output_scores,
            llm_response=response,
        )
