"""Synthex API server — paid API for decompiled code refinement.

Endpoints:
    POST /v1/refine  — Run the multi-generational refinement pipeline
    POST /v1/score   — Score a C file on quality metrics
    POST /v1/types   — Recover struct/type definitions from decompiled C
    GET  /v1/usage   — Check current usage against rate limits
    GET  /health     — Health check

Run:
    uvicorn synthex.api.server:app --host 0.0.0.0 --port 8000
"""

from __future__ import annotations

import tempfile
import time
from pathlib import Path

from fastapi import Depends, FastAPI
from fastapi.middleware.cors import CORSMiddleware

from synthex.api.auth import validate_key, record_usage, TIERS
from synthex.api.models import (
    RefineRequest,
    RefineResponse,
    GenerationOutput,
    ScoreRequest,
    ScoreResponse,
    ScoreResult,
    TypesRequest,
    TypesResponse,
    TypeHintOut,
    UsageResponse,
    ErrorResponse,
)

app = FastAPI(
    title="Synthex API",
    description="Multi-generational AI code refinement for decompiled binaries",
    version="0.2.0",
    docs_url="/docs",
    redoc_url="/redoc",
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.get("/health")
def health():
    return {"status": "ok", "version": "0.2.0"}


@app.post(
    "/v1/refine",
    response_model=RefineResponse,
    responses={401: {"model": ErrorResponse}, 429: {"model": ErrorResponse}},
)
def refine(req: RefineRequest, key_info: dict = Depends(validate_key)):
    """Run the multi-generational refinement pipeline on decompiled C code."""
    from synthex.pipeline import run_pipeline
    from synthex.scoring import score_code

    tier = key_info["tier_config"]

    # Enforce tier limits
    if len(req.code.encode()) > tier.max_code_bytes:
        from fastapi import HTTPException
        raise HTTPException(
            status_code=413,
            detail=f"Code exceeds {tier.max_code_bytes:,} byte limit for {tier.name} tier",
        )

    if req.generations > tier.generations:
        from fastapi import HTTPException
        raise HTTPException(
            status_code=403,
            detail=f"{tier.name} tier allows max {tier.generations} generation(s)",
        )

    # Run pipeline
    with tempfile.TemporaryDirectory() as tmpdir:
        result = run_pipeline(
            input_code=req.code,
            model=req.model,
            output_dir=tmpdir,
            generations=req.generations,
        )

    # Build response
    stages = []
    for gen in result.generations:
        stages.append(GenerationOutput(
            stage=gen.name,
            agent_type=gen.agent_type,
            generation=gen.generation,
            score=gen.scores.total_score,
            code=gen.code,
            exit_report=gen.exit_report.to_dict(),
        ))

    input_score = result.input_scores.total_score
    final_score = result.final_scores.total_score

    # Record usage
    record_usage(
        key_info["key_hash"],
        tokens=result.total_tokens,
        bytes_processed=len(req.code.encode()),
    )

    return RefineResponse(
        input_score=input_score,
        final_score=final_score,
        improvement=final_score - input_score,
        stages=stages,
        total_tokens=result.total_tokens,
        regression_detected=result.regression_detected,
    )


@app.post(
    "/v1/score",
    response_model=ScoreResponse,
    responses={401: {"model": ErrorResponse}},
)
def score(req: ScoreRequest, key_info: dict = Depends(validate_key)):
    """Score a C file on quality metrics. No LLM calls — deterministic."""
    from synthex.scoring import score_code

    breakdown = score_code(req.code)

    record_usage(key_info["key_hash"], bytes_processed=len(req.code.encode()))

    return ScoreResponse(
        score=ScoreResult(
            total_score=breakdown.total_score,
            lines=breakdown.lines,
            goto_count=breakdown.goto_count,
            meaningful_names=breakdown.meaningful_names,
            type_coverage=breakdown.type_coverage,
            comment_density=breakdown.comment_density,
            cyclomatic_complexity=breakdown.cyclomatic_complexity,
            magic_number_count=breakdown.magic_number_count,
            compiles=breakdown.compiles,
        )
    )


@app.post(
    "/v1/types",
    response_model=TypesResponse,
    responses={401: {"model": ErrorResponse}},
)
def recover_types(req: TypesRequest, key_info: dict = Depends(validate_key)):
    """Recover struct layouts and type definitions from decompiled C. No LLM calls."""
    from synthex.agents.type_recovery import analyze_types

    result = analyze_types(req.code)

    record_usage(key_info["key_hash"], bytes_processed=len(req.code.encode()))

    return TypesResponse(
        hints=[
            TypeHintOut(
                kind=h.kind,
                name=h.name,
                evidence=h.evidence,
                definition=h.definition,
            )
            for h in result.hints
        ],
        header=result.header,
        alloc_sizes=[[s, c] for s, c in result.alloc_sizes],
        pointer_strides=[[s, c] for s, c in result.pointer_strides],
    )


@app.get(
    "/v1/usage",
    response_model=UsageResponse,
    responses={401: {"model": ErrorResponse}},
)
def usage(key_info: dict = Depends(validate_key)):
    """Check current API usage against rate limits."""
    tier = key_info["tier_config"]
    day_usage = key_info["usage_today"]

    return UsageResponse(
        tier=key_info["tier"],
        owner=key_info["owner"],
        requests_today=day_usage.get("requests", 0),
        requests_limit=tier.requests_per_day,
        tokens_today=day_usage.get("tokens", 0),
        bytes_today=day_usage.get("bytes", 0),
    )
