"""Synthex CLI -- multi-generational code synthesis from the terminal."""

from __future__ import annotations

import os
import sys
from pathlib import Path

# Fix Windows Unicode rendering for Rich
if sys.platform == "win32":
    os.environ.setdefault("PYTHONIOENCODING", "utf-8")
    try:
        sys.stdout.reconfigure(encoding="utf-8")
        sys.stderr.reconfigure(encoding="utf-8")
    except (AttributeError, OSError):
        pass

import click
from rich.console import Console
from rich.panel import Panel
from rich.table import Table
from rich.text import Text
from rich.rule import Rule

from synthex.scoring import score_code, ScoreBreakdown

GOLD = "#c9a227"
DIM = "dim"
console = Console()


def _score_color(score: int) -> str:
    """Return a Rich color string based on score value."""
    if score >= 80:
        return "green"
    if score >= 60:
        return GOLD
    if score >= 40:
        return "yellow"
    return "red"


def _delta_str(delta: int) -> str:
    """Format a score delta with sign and color."""
    if delta > 0:
        return f"[green]+{delta}[/green]"
    if delta < 0:
        return f"[red]{delta}[/red]"
    return f"[{DIM}]+0[/{DIM}]"


def _print_header() -> None:
    console.print()
    title = Text("Synthex", style=f"bold {GOLD}")
    title.append(" -- Multi-Generational Code Synthesis", style=DIM)
    console.print(Panel(title, border_style=GOLD, padding=(0, 2)))
    console.print()


def _print_score_table(progression: list[tuple[str, int]]) -> None:
    """Print the final score progression table."""
    table = Table(
        title="Score Progression",
        title_style=f"bold {GOLD}",
        border_style=GOLD,
        show_lines=True,
    )
    table.add_column("Stage", style="bold")
    table.add_column("Score", justify="center")
    table.add_column("Delta", justify="center")

    prev_score = 0
    for i, (name, score) in enumerate(progression):
        score_style = _score_color(score)
        if i == 0:
            table.add_row(name, f"[{score_style}]{score}/100[/{score_style}]", "--")
        else:
            delta = score - prev_score
            table.add_row(
                name,
                f"[{score_style}]{score}/100[/{score_style}]",
                _delta_str(delta),
            )
        prev_score = score

    console.print(table)


def _print_breakdown(label: str, breakdown: ScoreBreakdown) -> None:
    """Print a detailed score breakdown."""
    table = Table(title=label, border_style=DIM, show_lines=False)
    table.add_column("Metric", style="bold")
    table.add_column("Value", justify="right")

    table.add_row("Lines", str(breakdown.lines))
    table.add_row("Goto statements", str(breakdown.goto_count))
    table.add_row(
        "Meaningful names",
        f"{breakdown.meaningful_names:.0%} ({breakdown.meaningful_identifier_count}/{breakdown.total_identifiers})",
    )
    table.add_row(
        "Type coverage",
        f"{breakdown.type_coverage:.0%} ({breakdown.typed_declarations}/{breakdown.total_declarations})",
    )
    table.add_row(
        "Comment density",
        f"{breakdown.comment_density:.1%} ({breakdown.comment_lines} lines)",
    )
    table.add_row("Cyclomatic complexity", str(breakdown.cyclomatic_complexity))
    table.add_row("Magic numbers", str(breakdown.magic_number_count))
    compile_str = "[green]YES[/green]" if breakdown.compiles else "[red]NO[/red]"
    if breakdown.compile_errors and breakdown.compile_errors != ["gcc not found"]:
        compile_str += f" ({len(breakdown.compile_errors)} errors)"
    table.add_row("Compiles (gcc -c)", compile_str)
    table.add_row("Total score", f"[bold {_score_color(breakdown.total_score)}]{breakdown.total_score}/100[/]")

    console.print(table)
    console.print()


@click.group()
@click.version_option(package_name="synthex")
def main() -> None:
    """Synthex -- multi-generational agent code synthesis."""


@main.command()
@click.argument("input_file", type=click.Path(exists=True))
@click.option(
    "--generations", "-g", default=3, type=click.IntRange(1, 3),
    help="Number of generations to run (1-3).",
)
@click.option(
    "--model", "-m", default="gpt-4o-mini",
    help="LLM model name (OpenAI-compatible).",
)
@click.option(
    "--output", "-o", default="synthex-output",
    help="Output directory for refined code.",
)
@click.option(
    "--base-url", default=None,
    help="OpenAI-compatible API base URL (e.g., http://localhost:11434/v1 for Ollama).",
)
def refine(
    input_file: str,
    generations: int,
    model: str,
    output: str,
    base_url: str | None,
) -> None:
    """Refine decompiled C code through multiple agent generations."""
    from synthex.pipeline import run_pipeline

    input_path = Path(input_file)
    input_code = input_path.read_text()
    input_scores = score_code(input_code)

    _print_header()

    console.print(
        f"  Input: [bold]{input_path.name}[/bold] "
        f"({input_scores.lines} lines, score: "
        f"[{_score_color(input_scores.total_score)}]{input_scores.total_score}/100[/])"
    )
    console.print(f"  Model: [{DIM}]{model}[/{DIM}]")
    console.print(f"  Generations: [{DIM}]{generations}[/{DIM}]")
    console.print()

    try:
        result = run_pipeline(
            input_code=input_code,
            model=model,
            output_dir=output,
            generations=generations,
            base_url=base_url,
        )
    except Exception as exc:
        console.print(f"[red bold]Error:[/red bold] {exc}")
        sys.exit(1)

    # Print per-generation results
    prev_score = input_scores.total_score
    for gen in result.generations:
        score = gen.scores.total_score
        delta = score - prev_score

        console.print(Rule(f"[bold]{gen.name}[/bold]", style=GOLD))

        # Print what worked
        for item in gen.exit_report.what_worked:
            console.print(f"  [green]v[/green] {item}")
        for item in gen.exit_report.what_failed:
            console.print(f"  [red]x[/red] {item}")

        if gen.exit_report.emergent_skill:
            skill_status = (
                "[green]exercised[/green]"
                if gen.exit_report.emergent_skill_exercised
                else f"[{DIM}]theoretical[/{DIM}]"
            )
            console.print(f"  Emergent skill: {gen.exit_report.emergent_skill} ({skill_status})")

        console.print(
            f"  Score: [{_score_color(prev_score)}]{prev_score}[/] -> "
            f"[{_score_color(score)}]{score}[/] ({_delta_str(delta)})"
        )
        console.print()
        prev_score = score

    # Regression warnings
    if result.regression_detected:
        console.print(Panel(
            "\n".join(f"[red]![/red] {d}" for d in result.regression_details),
            title="[red bold]Regression Detected[/red bold]",
            border_style="red",
        ))
        console.print()

    # Final summary
    total_delta = result.final_scores.total_score - result.input_scores.total_score
    console.print(Rule(style=GOLD))
    console.print(
        f"  [bold]Final:[/bold] {result.input_scores.total_score} -> "
        f"[bold {_score_color(result.final_scores.total_score)}]"
        f"{result.final_scores.total_score}[/] "
        f"({_delta_str(total_delta)} across {len(result.generations)} generation(s))"
    )
    console.print(f"  Tokens: [{DIM}]{result.total_tokens:,}[/{DIM}]")
    console.print(f"  Output: [{DIM}]{output}/[/{DIM}]")
    console.print()

    _print_score_table(result.score_progression)


@main.command()
@click.argument("file", type=click.Path(exists=True))
def score(file: str) -> None:
    """Score a single C file on code quality metrics."""
    code = Path(file).read_text()
    scores = score_code(code)

    _print_header()
    _print_breakdown(f"Score: {Path(file).name}", scores)


@main.command()
@click.argument("file1", type=click.Path(exists=True))
@click.argument("file2", type=click.Path(exists=True))
def compare(file1: str, file2: str) -> None:
    """Compare two C files side by side on quality metrics."""
    code1 = Path(file1).read_text()
    code2 = Path(file2).read_text()
    s1 = score_code(code1)
    s2 = score_code(code2)

    _print_header()

    table = Table(
        title="Comparison",
        title_style=f"bold {GOLD}",
        border_style=GOLD,
        show_lines=True,
    )
    table.add_column("Metric", style="bold")
    table.add_column(Path(file1).name, justify="right")
    table.add_column(Path(file2).name, justify="right")
    table.add_column("Delta", justify="right")

    metrics = [
        ("Lines", s1.lines, s2.lines, True),        # lower is better
        ("Gotos", s1.goto_count, s2.goto_count, True),
        ("Meaningful names", f"{s1.meaningful_names:.0%}", f"{s2.meaningful_names:.0%}", None),
        ("Type coverage", f"{s1.type_coverage:.0%}", f"{s2.type_coverage:.0%}", None),
        ("Comment density", f"{s1.comment_density:.1%}", f"{s2.comment_density:.1%}", None),
        ("Complexity", s1.cyclomatic_complexity, s2.cyclomatic_complexity, True),
        ("Total score", s1.total_score, s2.total_score, False),  # higher is better
    ]

    for name, v1, v2, lower_better in metrics:
        if isinstance(v1, (int, float)) and isinstance(v2, (int, float)):
            delta = v2 - v1
            delta_s = _delta_str(delta) if not lower_better else _delta_str(-delta)
            table.add_row(name, str(v1), str(v2), delta_s)
        else:
            table.add_row(name, str(v1), str(v2), "--")

    console.print(table)
    console.print()


@main.command()
@click.argument("directory", type=click.Path(exists=True))
@click.option("--model", default="gpt-4o-mini", help="LLM model name.")
@click.option("--output", default="benchmark-output", help="Output directory.")
@click.option("--base-url", default=None, help="OpenAI-compatible API base URL.")
def benchmark(directory: str, model: str, output: str, base_url: str | None) -> None:
    """Run A/B benchmark: generational pipeline vs budget-matched control.

    DIRECTORY should contain .c files (one function each).
    Control arm: 3 sequential normalizer passes, no exit reports, no fork.
    Same token budget. Reports delta between arms.
    """
    import json as json_mod
    from synthex.benchmark import run_benchmark

    input_dir = Path(directory)
    output_dir = Path(output)
    output_dir.mkdir(parents=True, exist_ok=True)

    c_files = sorted(input_dir.glob("*.c"))
    if not c_files:
        console.print(f"[red]No .c files found in {directory}[/red]")
        raise SystemExit(1)

    _print_header()
    console.print(f"  Benchmark: {len(c_files)} functions")
    console.print(f"  Model: {model}")
    console.print(f"  Arms: [bold]experimental[/bold] (G1->G2a+G2b->G3) vs [bold]control[/bold] (3x normalizer)")
    console.print()

    results = []
    for i, f in enumerate(c_files, 1):
        code = f.read_text(encoding="utf-8", errors="replace")
        fname = f.stem
        console.print(Rule(f"[{i}/{len(c_files)}] {fname}", style=DIM))

        try:
            r = run_benchmark(
                code=code,
                function_name=fname,
                model=model,
                output_dir=output_dir,
                base_url=base_url,
            )
            results.append(r)

            adv_color = "green" if r.advantage > 0 else ("red" if r.advantage < 0 else DIM)
            console.print(
                f"  Input: {r.input_score}  "
                f"Exp: {r.experimental.delta:+d} ({r.experimental.compile_rate:.0%} compile)  "
                f"Ctrl: {r.control.delta:+d} ({r.control.compile_rate:.0%} compile)  "
                f"Advantage: [{adv_color}]{r.advantage:+d}[/{adv_color}]"
            )
        except Exception as e:
            console.print(f"  [red]Error: {e}[/red]")
        console.print()

    if not results:
        console.print("[red]No results.[/red]")
        return

    # Summary table
    console.print(Rule(style=GOLD))
    table = Table(title="Benchmark Results", title_style=f"bold {GOLD}",
                  border_style=GOLD, show_lines=True)
    table.add_column("Function", style="bold")
    table.add_column("Input", justify="right")
    table.add_column("Exp Delta", justify="right")
    table.add_column("Ctrl Delta", justify="right")
    table.add_column("Advantage", justify="right")
    table.add_column("Exp Compile", justify="right")
    table.add_column("Ctrl Compile", justify="right")

    for r in results:
        adv_color = "green" if r.advantage > 0 else ("red" if r.advantage < 0 else DIM)
        table.add_row(
            r.function_name,
            str(r.input_score),
            f"{r.experimental.delta:+d}",
            f"{r.control.delta:+d}",
            f"[{adv_color}]{r.advantage:+d}[/{adv_color}]",
            f"{r.experimental.compile_rate:.0%}",
            f"{r.control.compile_rate:.0%}",
        )

    avg_advantage = sum(r.advantage for r in results) / len(results)
    wins = sum(1 for r in results if r.advantage > 0)
    losses = sum(1 for r in results if r.advantage < 0)
    ties = sum(1 for r in results if r.advantage == 0)
    avg_exp_compile = sum(r.experimental.compile_rate for r in results) / len(results)
    avg_ctrl_compile = sum(r.control.compile_rate for r in results) / len(results)
    exp_tokens = sum(r.experimental.total_tokens for r in results)
    ctrl_tokens = sum(r.control.total_tokens for r in results)

    table.add_row(
        "[bold]MEAN[/bold]", "",
        f"{sum(r.experimental.delta for r in results) / len(results):+.1f}",
        f"{sum(r.control.delta for r in results) / len(results):+.1f}",
        f"[bold]{avg_advantage:+.1f}[/bold]",
        f"{avg_exp_compile:.0%}",
        f"{avg_ctrl_compile:.0%}",
    )

    console.print(table)
    console.print()
    console.print(f"  Record: [green]{wins}W[/green] / [red]{losses}L[/red] / [{DIM}]{ties}T[/{DIM}]")
    console.print(f"  Mean advantage: {avg_advantage:+.1f} points")
    console.print(f"  Compile rate: experimental {avg_exp_compile:.0%} vs control {avg_ctrl_compile:.0%}")
    console.print(f"  Tokens: experimental {exp_tokens:,} vs control {ctrl_tokens:,}")
    console.print()

    report = {
        "model": model,
        "functions": len(results),
        "wins": wins, "losses": losses, "ties": ties,
        "mean_advantage": round(avg_advantage, 1),
        "experimental_compile_rate": round(avg_exp_compile, 3),
        "control_compile_rate": round(avg_ctrl_compile, 3),
        "experimental_tokens": exp_tokens,
        "control_tokens": ctrl_tokens,
        "per_function": [
            {"name": r.function_name, "input_score": r.input_score,
             "experimental_delta": r.experimental.delta, "control_delta": r.control.delta,
             "advantage": r.advantage}
            for r in results
        ],
    }
    report_path = output_dir / "benchmark-report.json"
    report_path.write_text(json_mod.dumps(report, indent=2))
    console.print(f"  Report: [{DIM}]{report_path}[/{DIM}]")


@main.command()
@click.argument("input_file", type=click.Path(exists=True))
@click.option(
    "--output", "-o", default="reconstructed",
    help="Output directory for the reconstructed project.",
)
@click.option(
    "--model", "-m", default="gpt-4o-mini",
    help="LLM model name (OpenAI-compatible).",
)
@click.option(
    "--base-url", default=None,
    help="OpenAI-compatible API base URL.",
)
def reconstruct(
    input_file: str,
    output: str,
    model: str,
    base_url: str | None,
) -> None:
    """Reconstruct a compilable C project from Ghidra decompiled output.

    Takes a decompiled .c file (e.g., from Ghidra) and produces a single
    compilable, runnable C project by identifying user functions, cleaning
    them via LLM, and assembling the result.
    """
    from synthex.reconstruct import reconstruct as run_reconstruct

    _print_header()

    input_path = Path(input_file)
    console.print(f"  Input: [bold]{input_path.name}[/bold]")
    console.print(f"  Model: [{DIM}]{model}[/{DIM}]")
    console.print(f"  Output: [{DIM}]{output}/[/{DIM}]")
    console.print()

    def on_progress(stage: str, detail: str) -> None:
        stage_icons = {
            "parse": "1/6 Parse",
            "filter": "2/6 Filter",
            "reconstruct": "3/6 Reconstruct",
            "skip": "3/6 Reconstruct",
            "assemble": "4/6 Assemble",
            "compile": "5/6 Compile",
            "fix": "5/6 Fix",
            "test": "6/6 Test",
        }
        label = stage_icons.get(stage, stage)
        console.print(f"  [{DIM}][{label}][/{DIM}] {detail}")

    try:
        result = run_reconstruct(
            decompiled_file=input_file,
            output_dir=output,
            model=model,
            base_url=base_url,
            on_progress=on_progress,
        )
    except Exception as exc:
        console.print(f"\n[red bold]Error:[/red bold] {exc}")
        raise SystemExit(1)

    # Summary
    console.print()
    console.print(Rule(style=GOLD))
    console.print(f"  [bold]Functions reconstructed:[/bold] {len(result.functions)}")

    # Show function mapping
    for func in result.functions:
        console.print(f"    {func.original_name} -> [bold]{func.new_name}[/bold]")

    console.print()

    # Compilation result
    if result.compiled:
        console.print(f"  Compilation: [green]SUCCESS[/green] (attempt {result.compile_attempts})")
    else:
        console.print(f"  Compilation: [red]FAILED[/red] after {result.compile_attempts} attempts")
        if result.compile_errors:
            for err in result.compile_errors[:10]:
                console.print(f"    [red]{err}[/red]")

    # Differential test result
    if result.diff_test_passed is True:
        console.print(f"  Differential test: [green]PASSED[/green] -- output matches original binary")
    elif result.diff_test_passed is False:
        console.print(f"  Differential test: [red]FAILED[/red]")
        if result.diff_test_output:
            console.print(f"    {result.diff_test_output[:200]}")
    else:
        console.print(f"  Differential test: [{DIM}]skipped[/{DIM}]")

    console.print(f"  Tokens: [{DIM}]{result.total_tokens:,}[/{DIM}]")
    console.print(f"  Output: [{DIM}]{result.output_dir}/reconstructed.c[/{DIM}]")
    console.print()
