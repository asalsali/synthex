"""Pydantic models for Synthex API request/response schemas."""

from __future__ import annotations

from pydantic import BaseModel, Field


class RefineRequest(BaseModel):
    code: str = Field(..., description="Decompiled C source code to refine")
    generations: int = Field(default=3, ge=1, le=3, description="Number of generations (1-3)")
    model: str = Field(default="gpt-4o-mini", description="LLM model name")

    model_config = {
        "json_schema_extra": {
            "examples": [{
                "code": "undefined8 FUN_00401000(long param_1) { ... }",
                "generations": 3,
                "model": "gpt-4o-mini",
            }]
        }
    }


class ScoreResult(BaseModel):
    total_score: int
    lines: int
    goto_count: int
    meaningful_names: float
    type_coverage: float
    comment_density: float
    cyclomatic_complexity: int
    magic_number_count: int
    compiles: bool


class GenerationOutput(BaseModel):
    stage: str
    agent_type: str
    generation: int
    score: int
    code: str
    exit_report: dict


class RefineResponse(BaseModel):
    input_score: int
    final_score: int
    improvement: int
    stages: list[GenerationOutput]
    total_tokens: int
    regression_detected: bool


class ScoreRequest(BaseModel):
    code: str = Field(..., description="C source code to score")


class ScoreResponse(BaseModel):
    score: ScoreResult


class TypesRequest(BaseModel):
    code: str = Field(..., description="Decompiled C code for type recovery")


class TypeHintOut(BaseModel):
    kind: str
    name: str
    evidence: str
    definition: str


class TypesResponse(BaseModel):
    hints: list[TypeHintOut]
    header: str
    alloc_sizes: list[list]
    pointer_strides: list[list]


class UsageResponse(BaseModel):
    tier: str
    owner: str
    requests_today: int
    requests_limit: int
    tokens_today: int
    bytes_today: int


class ErrorResponse(BaseModel):
    detail: str
