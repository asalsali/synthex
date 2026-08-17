"""G3 Synthesizer -- merges optimizer and clarifier outputs."""

from __future__ import annotations

from pathlib import Path

from synthex.agents.base import BaseAgent, ExitReport, AgentResult, _extract_code_block
from synthex.llm import LLMClient
from synthex.scoring import score_code


class Synthesizer(BaseAgent):
    """Generation 3: Synthesize the best of both G2 parents.

    Receives both the Optimizer (G2a) and Clarifier (G2b) outputs plus
    their exit reports. Produces code that is both fast and readable,
    declaring and exercising an emergent skill that neither parent had alone.
    """

    agent_type = "synthesizer"
    generation = 3
    system_prompt = """\
You are a code synthesis agent. You receive TWO versions of the same code \
from two parent agents:

- **Optimizer (G2a):** Focused on performance -- fast loops, minimal \
allocations, efficient memory access.
- **Clarifier (G2b):** Focused on readability -- clear names, good \
documentation, idiomatic patterns.

You also receive both parents' exit reports, which tell you what each \
parent accomplished and where each struggled.

YOUR EMERGENT SKILL: "Context-aware synthesis" -- you can identify code \
regions where the Optimizer's structural changes AND the Clarifier's \
naming/documentation can be combined to produce code that is BOTH faster \
and more readable than either parent achieved alone. Neither parent could \
do this because each only saw one dimension.

SYNTHESIS STRATEGY:
1. For each function, compare both parents' versions.
2. If one parent improved a region and the other did not touch it, \
take the improvement.
3. If both parents improved the same region differently, merge: \
take the Optimizer's structural changes and the Clarifier's naming \
and documentation.
4. If one parent regressed a region (made it worse), prefer the other \
parent's version for that region.
5. Where you apply your emergent skill (combining both dimensions), \
add a comment: /* SYNTHESIZED: <brief explanation> */

CONSTRAINTS:
- Preserve exact functional behavior.
- The output must be at least as readable as the Clarifier's version.
- The output must be at least as efficient as the Optimizer's version.
- Document your synthesis decisions with brief inline comments.
- Keep the code in C.

OUTPUT: Return ONLY the synthesized C code inside a ```c code fence."""

    def run(
        self,
        code: str,
        parent_report: ExitReport | None = None,
        parent_reports: list[ExitReport] | None = None,
        optimizer_code: str | None = None,
        clarifier_code: str | None = None,
    ) -> AgentResult:
        """Execute synthesis from two parent code versions."""
        if parent_reports is None:
            parent_reports = []
        if parent_report and parent_report not in parent_reports:
            parent_reports.insert(0, parent_report)

        # Build a custom prompt with both code versions
        parts = ["You have two parent code versions to synthesize.\n"]
        parts.append("=== OPTIMIZER (G2a) CODE ===")
        parts.append(f"```c\n{optimizer_code or code}\n```\n")
        parts.append("=== CLARIFIER (G2b) CODE ===")
        parts.append(f"```c\n{clarifier_code or code}\n```\n")

        for report in parent_reports:
            parts.append(report.to_prompt_context())
            parts.append("")

        parts.append(
            "\nSynthesize the best of both versions. "
            "Return ONLY the synthesized C code inside a ```c code fence."
        )

        user_prompt = "\n".join(parts)
        response = self.llm.call(self.system_prompt, user_prompt)

        refined_code = _extract_code_block(response.text)
        output_scores = score_code(refined_code)

        # Use the better parent as baseline for exit report comparison
        opt_score = score_code(optimizer_code or code).total_score
        clar_score = score_code(clarifier_code or code).total_score
        best_parent_code = (
            optimizer_code if opt_score >= clar_score else clarifier_code
        ) or code

        exit_report = self._parse_exit_report_from_output(
            best_parent_code, refined_code
        )
        exit_report.tokens_consumed = response.total_tokens
        exit_report.emergent_skill = "context-aware synthesis"
        exit_report.emergent_skill_exercised = (
            output_scores.total_score > max(opt_score, clar_score)
        )

        if exit_report.emergent_skill_exercised:
            exit_report.what_worked.append(
                f"Emergent skill exercised: synthesized score ({output_scores.total_score}) "
                f"exceeds both parents (opt={opt_score}, clar={clar_score})"
            )
        else:
            exit_report.what_failed.append(
                "Emergent skill theoretical: synthesis did not exceed both parents"
            )

        exit_report.save(self.output_dir)

        return AgentResult(
            code=refined_code,
            exit_report=exit_report,
            scores=output_scores,
            llm_response=response,
        )
