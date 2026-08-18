# Synthex

Multi-generational agent code synthesis. Takes decompiled or legacy C code and runs it through multiple LLM agent generations -- each reading the previous generation's exit report to produce progressively cleaner, more readable code.

**Proven result:** Ghidra decompiled C scored 20/100 improved to 78/100 across 3 generations using gpt-4o-mini (~11K tokens).

## Install

```bash
pip install synthex
```

Requires an OpenAI-compatible API key:

```bash
export OPENAI_API_KEY=sk-...
```

### Using with Ollama (local, free)

```bash
ollama pull codellama:34b
synthex refine input.c --model codellama:34b --base-url http://localhost:11434/v1
```

## Quick Start

```bash
# Score a decompiled C file
synthex score examples/sample_decompiled.c

# Refine through 3 generations
synthex refine examples/sample_decompiled.c --generations 3

# Compare two files side-by-side
synthex compare examples/sample_decompiled.c synthex-output/g3-synthesizer.c
```

## How Generations Work

Each generation is a specialized agent that reads code and the previous agent's structured exit report. The exit report is the gradient -- it tells the next generation what worked, what failed, and where gaps remain.

```
Input (raw Ghidra output, score: 20/100)
  |
  v
G1 Normalizer ── rename vars, remove gotos, recover types
  |                score: 20 -> 66 (+46)
  |                exit report: what worked, what failed, gaps
  v
G2a Optimizer ── reads G1 report, optimizes for speed
  |                score: 66 -> 67 (+1)
  |
G2b Clarifier ── reads G1 report, maximizes readability
  |                score: 67 -> 77 (+10)
  v
G3 Synthesizer ── reads BOTH G2 reports + code, merges best of both
                   score: 77 -> 78 (+1)
                   declares emergent skill: "context-aware synthesis"
                   skill verified: YES (exceeds both parents)
```

### Agent Synthesis (Covenant Pattern)

G3 is not just another iteration -- it is a **synthesis agent** that receives two parent versions of the same code (one optimized for speed, one for readability) and must produce code that is better than both. It declares an "emergent skill" -- a capability that neither parent had alone -- and the system verifies whether that skill produced measurably better output.

This pattern comes from the [Covenant Framework](https://github.com/asalsali/covenant-framework)'s Synthesis Law (Section XIV).

## Output

After running `synthex refine`, find results in `synthex-output/`:

```
synthex-output/
  g0-input.c           -- original input
  g1-normalizer.c      -- G1 output
  g2a-optimizer.c      -- G2a output (speed focus)
  g2b-clarifier.c      -- G2b output (readability focus)
  g3-synthesizer.c     -- final synthesized output
  inheritance/         -- exit reports from each agent (JSON)
```

## Scoring

Files are scored 0-100 based on six metrics:

| Metric | Weight | Direction | What it measures |
|---|---|---|---|
| Goto statements | 20 | Fewer = better | Control flow structure |
| Meaningful names | 20 | Higher ratio = better | Variable naming quality |
| Comment density | 20 | Higher = better (cap 25%) | Documentation coverage |
| Line count | 10 | Shorter = better | Code conciseness |
| Type coverage | 10 | Higher ratio = better | Explicit type annotations |
| Cyclomatic complexity | 10 | Lower = better | Code simplicity |
| Magic numbers | 10 | Fewer = better | Named constants usage |

## Exit Reports

Each agent writes a structured exit report (JSON) containing:
- `whatWorked` -- transformations that improved the code
- `whatFailed` -- attempted improvements that didn't work
- `recommendations` -- hints for the next generation
- `gaps` -- areas of uncertainty
- `emergentSkill` (G3 only) -- the declared synthesis capability
- `emergentSkillExercised` (G3 only) -- whether the skill produced measurably better output

## Architecture

Synthex uses a **tools-first, governance-informed** architecture:

- **No Covenant dependency** -- standalone Python package
- **Covenant-inspired patterns** -- exit reports, generational inheritance, dual-parent synthesis
- **Any OpenAI-compatible API** -- works with OpenAI, Ollama, vLLM, Together, etc.
- **Deterministic scoring** -- no LLM involved in quality measurement

## License

MIT
