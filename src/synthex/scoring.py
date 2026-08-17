"""Code quality scoring for decompiled C refinement."""

from __future__ import annotations

import re
from dataclasses import dataclass, field


# Patterns matching Ghidra-style generated names
_GENERATED_NAME_PATTERNS = [
    re.compile(r"^[iu]Var\d+$"),
    re.compile(r"^local_[0-9a-fA-F]+$"),
    re.compile(r"^param_\d+$"),
    re.compile(r"^pvVar\d+$"),
    re.compile(r"^lVar\d+$"),
    re.compile(r"^uVar\d+$"),
    re.compile(r"^[A-Z]+_[0-9a-fA-F]{6,}$"),   # LAB_004010f0, FUN_00401000
    re.compile(r"^FUN_[0-9a-fA-F]+$"),
    re.compile(r"^DAT_[0-9a-fA-F]+$"),
    re.compile(r"^undefined\d*$"),
]

# C keywords and types to exclude from identifier analysis
_C_KEYWORDS = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "return", "short", "signed",
    "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned",
    "void", "volatile", "while", "NULL", "nullptr",
}


@dataclass
class ScoreBreakdown:
    """Detailed score breakdown across all metrics."""

    lines: int = 0
    goto_count: int = 0
    meaningful_names: float = 0.0
    type_coverage: float = 0.0
    comment_density: float = 0.0
    cyclomatic_complexity: int = 0
    total_identifiers: int = 0
    meaningful_identifier_count: int = 0
    typed_declarations: int = 0
    total_declarations: int = 0
    comment_lines: int = 0
    magic_number_count: int = 0

    @property
    def total_score(self) -> int:
        """Compute composite score 0-100."""
        score = 0.0

        # Line count: shorter is better (max 10 points)
        # Baseline: 200 lines = 0 points, 30 lines = 10 points
        line_score = max(0, min(10, 10 * (1 - (self.lines - 30) / 170)))
        score += line_score

        # Goto count: fewer is better (max 20 points)
        if self.goto_count == 0:
            score += 20
        else:
            score += max(0, 20 - self.goto_count * 4)

        # Meaningful names: higher ratio is better (max 20 points)
        score += self.meaningful_names * 20

        # Type coverage: higher ratio is better (max 10 points)
        score += self.type_coverage * 10

        # Comment density: higher is better up to 0.25 (max 20 points)
        # This rewards clarifier-style documentation heavily
        density_capped = min(self.comment_density, 0.25)
        score += (density_capped / 0.25) * 20

        # Cyclomatic complexity: lower is better (max 10 points)
        cx_score = max(0, min(10, 10 * (1 - (self.cyclomatic_complexity - 3) / 17)))
        score += cx_score

        # Magic number penalty (max 10 points)
        # Fewer raw hex/large numeric literals = better (rewards named constants)
        score += max(0, 10 - self.magic_number_count * 2)

        return max(0, min(100, round(score)))


def _extract_identifiers(code: str) -> list[str]:
    """Extract C identifiers from code, excluding keywords and types."""
    # Match word-boundary identifiers
    raw = re.findall(r"\b([a-zA-Z_][a-zA-Z0-9_]*)\b", code)
    # Filter keywords, standard types, and very short tokens
    return [
        name for name in raw
        if name not in _C_KEYWORDS
        and not name.startswith("__")
        and len(name) > 1
    ]


def _is_meaningful_name(name: str) -> bool:
    """Check if an identifier is meaningful (not auto-generated)."""
    if len(name) <= 3:
        return False
    for pattern in _GENERATED_NAME_PATTERNS:
        if pattern.match(name):
            return False
    return True


def _count_gotos(code: str) -> int:
    """Count goto statements in the code."""
    return len(re.findall(r"\bgoto\b", code))


def _count_typed_declarations(code: str) -> tuple[int, int]:
    """Count typed vs total variable declarations.

    Returns (typed_count, total_count).
    """
    # Match lines that look like variable declarations
    decl_pattern = re.compile(
        r"^\s*(?:(?:const|static|volatile|unsigned|signed|long|short)\s+)*"
        r"(\w+)\s+\**\s*(\w+)",
        re.MULTILINE,
    )
    matches = decl_pattern.findall(code)

    total = 0
    typed = 0
    weak_types = {"undefined", "undefined8", "undefined4", "undefined2", "undefined1"}

    for type_name, _var_name in matches:
        if type_name in _C_KEYWORDS and type_name not in {"int", "char", "long",
                                                           "short", "float", "double",
                                                           "void", "unsigned", "signed"}:
            continue
        total += 1
        if type_name not in weak_types and type_name != "void":
            typed += 1

    return typed, max(total, 1)


def _count_comment_lines(code: str) -> int:
    """Count lines containing comments."""
    count = 0
    in_block = False
    for line in code.splitlines():
        stripped = line.strip()
        if in_block:
            count += 1
            if "*/" in stripped:
                in_block = False
            continue
        if stripped.startswith("//"):
            count += 1
        elif stripped.startswith("/*"):
            count += 1
            if "*/" not in stripped:
                in_block = True
        elif "/*" in stripped:
            count += 1
            if "*/" not in stripped:
                in_block = True
    return count


def _cyclomatic_complexity(code: str) -> int:
    """Estimate cyclomatic complexity from control flow keywords."""
    complexity = 1  # base complexity
    for kw in ["if", "else", "for", "while", "switch", "case"]:
        complexity += len(re.findall(rf"\b{kw}\b", code))
    complexity += code.count("&&") + code.count("||") + code.count("?")
    return complexity


def score_code(code: str) -> ScoreBreakdown:
    """Score a C code file on quality metrics. Returns ScoreBreakdown."""
    lines = code.count("\n") + 1
    identifiers = _extract_identifiers(code)

    # Deduplicate for name quality analysis
    unique_ids = list(set(identifiers))
    meaningful_count = sum(1 for name in unique_ids if _is_meaningful_name(name))
    meaningful_ratio = meaningful_count / max(len(unique_ids), 1)

    goto_count = _count_gotos(code)
    typed, total_decls = _count_typed_declarations(code)
    type_ratio = typed / max(total_decls, 1)
    comment_lines = _count_comment_lines(code)
    comment_density = comment_lines / max(lines, 1)
    complexity = _cyclomatic_complexity(code)
    magic_numbers = len(re.findall(r"\b0x[0-9a-fA-F]{3,}\b", code))

    return ScoreBreakdown(
        lines=lines,
        goto_count=goto_count,
        meaningful_names=meaningful_ratio,
        type_coverage=type_ratio,
        comment_density=comment_density,
        cyclomatic_complexity=complexity,
        total_identifiers=len(unique_ids),
        meaningful_identifier_count=meaningful_count,
        typed_declarations=typed,
        total_declarations=total_decls,
        comment_lines=comment_lines,
        magic_number_count=magic_numbers,
    )
