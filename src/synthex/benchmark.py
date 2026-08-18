"""Benchmark harness: generational pipeline vs budget-matched control.

Control arm: 3 sequential normalizer passes, no exit reports, no fork.
Same total token budget as the experimental arm.

Reports delta between arms, not absolute scores.
"""

from __future__ import annotations

import json
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from synthex.agents.base import _extract_code_block
from synthex.llm import LLMClient
from synthex.pipeline import run_pipeline
from synthex.scoring import score_code


@dataclass
class ArmResult:
    """Result of one experimental arm."""
    name: str
    input_score: int
    final_score: int
    delta: int
    compile_rate: float  # fraction of stages that compiled
    scores_per_stage: list[int] = field(default_factory=list)
    compiles_per_stage: list[bool] = field(default_factory=list)
    total_tokens: int = 0
    duration_seconds: float = 0.0


@dataclass
class BenchmarkResult:
    """Comparison between experimental and control arms."""
    function_name: str
    input_score: int
    experimental: ArmResult
    control: ArmResult
    advantage: int  # experimental.delta - control.delta
    compile_advantage: float  # experimental.compile_rate - control.compile_rate


def run_control_arm(
    code: str,
    llm: LLMClient,
    output_dir: Path,
    passes: int = 3,
) -> ArmResult:
    """Run budget-matched control: N normalizer passes, no reports, no fork.

    Each pass sends the same system prompt as the normalizer but does NOT
    include exit reports. The next pass receives the prior output as input,
    nothing else. This isolates the value of exit-report-driven feedback
    from raw iterative refinement.
    """
    input_scores = score_code(code)
    current_code = code
    scores = [input_scores.total_score]
    compiles = [input_scores.compiles]
    total_tokens = 0
    start = time.time()

    # Reuse normalizer's system prompt but strip any exit report references
    control_system = """\
You are a decompiled code normalizer. Your job is to take raw decompiler \
output and produce clean, readable C code.

TRANSFORMATIONS (apply all that are relevant):
1. RENAME VARIABLES: Replace auto-generated names with meaningful names.
2. REMOVE GOTOS: Convert goto-based control flow to structured statements.
3. RECOVER TYPES: Replace undefined types with specific types.
4. REMOVE DEAD CODE: Eliminate unreachable code and redundant variables.
5. ADD BASIC COMMENTS: Add a brief comment at the top of each function.
6. CLEAN UP FORMATTING: Consistent indentation and logical grouping.

CONSTRAINTS:
- Preserve the exact functional behavior of the code.
- Keep the code in C.

OUTPUT: Return ONLY the refined C code inside a ```c code fence."""

    for i in range(passes):
        user_prompt = (
            f"Here is the C code to refine:\n\n```c\n{current_code}\n```\n\n"
            "Return ONLY the refined C code inside a ```c code fence. "
            "Do not include explanations outside the code fence."
        )
        response = llm.call(control_system, user_prompt)
        total_tokens += response.total_tokens

        refined = _extract_code_block(response.text)
        stage_scores = score_code(refined)
        scores.append(stage_scores.total_score)
        compiles.append(stage_scores.compiles)
        current_code = refined

        # Save control output
        out_path = output_dir / f"control-pass-{i + 1}.c"
        out_path.write_text(refined, encoding="utf-8")

    duration = time.time() - start
    final_score = scores[-1]
    compile_rate = sum(1 for c in compiles[1:] if c) / max(len(compiles) - 1, 1)

    return ArmResult(
        name="control",
        input_score=scores[0],
        final_score=final_score,
        delta=final_score - scores[0],
        compile_rate=compile_rate,
        scores_per_stage=scores,
        compiles_per_stage=compiles,
        total_tokens=total_tokens,
        duration_seconds=duration,
    )


def run_experimental_arm(
    code: str,
    model: str,
    output_dir: Path,
    base_url: str | None = None,
    function_map: dict[str, str] | None = None,
) -> ArmResult:
    """Run the full generational pipeline (G1 -> G2a+G2b -> G3)."""
    start = time.time()

    result = run_pipeline(
        input_code=code,
        model=model,
        output_dir=output_dir,
        generations=3,
        base_url=base_url,
        function_map=function_map,
    )
    duration = time.time() - start

    scores = [score for _, score in result.score_progression]
    # Get compile status from generation scores
    compiles_list = [result.input_scores.compiles]
    for gen in result.generations:
        compiles_list.append(gen.scores.compiles)

    final_score = scores[-1]
    gen_compiles = compiles_list[1:]  # exclude input
    compile_rate = sum(1 for c in gen_compiles if c) / max(len(gen_compiles), 1)

    return ArmResult(
        name="experimental",
        input_score=scores[0],
        final_score=final_score,
        delta=final_score - scores[0],
        compile_rate=compile_rate,
        scores_per_stage=scores,
        compiles_per_stage=compiles_list,
        total_tokens=result.total_tokens,
        duration_seconds=duration,
    )


def run_benchmark(
    code: str,
    function_name: str,
    model: str = "gpt-4o-mini",
    output_dir: Path = Path("benchmark-output"),
    base_url: str | None = None,
) -> BenchmarkResult:
    """Run both arms and compare."""
    func_dir = output_dir / function_name.replace(" ", "_")
    exp_dir = func_dir / "experimental"
    ctrl_dir = func_dir / "control"
    exp_dir.mkdir(parents=True, exist_ok=True)
    ctrl_dir.mkdir(parents=True, exist_ok=True)

    llm = LLMClient(model=model, base_url=base_url)
    input_score = score_code(code).total_score

    experimental = run_experimental_arm(code, model, exp_dir, base_url)
    control = run_control_arm(code, llm, ctrl_dir, passes=3)

    return BenchmarkResult(
        function_name=function_name,
        input_score=input_score,
        experimental=experimental,
        control=control,
        advantage=experimental.delta - control.delta,
        compile_advantage=experimental.compile_rate - control.compile_rate,
    )
