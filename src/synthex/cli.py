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
