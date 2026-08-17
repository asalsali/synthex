"""LLM client wrapper for Synthex agents."""

from __future__ import annotations

import os
from dataclasses import dataclass

from openai import OpenAI


@dataclass
class LLMResponse:
    """Response from an LLM call."""

    text: str
    prompt_tokens: int
    completion_tokens: int
    model: str

    @property
    def total_tokens(self) -> int:
        return self.prompt_tokens + self.completion_tokens


class LLMClient:
    """Thin wrapper around the OpenAI SDK for agent calls."""

    def __init__(self, model: str = "gpt-4o-mini", base_url: str | None = None):
        self.model = model
        self._client = OpenAI(
            api_key=os.environ.get("OPENAI_API_KEY", "sk-placeholder"),
            base_url=base_url or os.environ.get("OPENAI_BASE_URL"),
        )
        self.total_prompt_tokens = 0
        self.total_completion_tokens = 0

    def call(self, system_prompt: str, user_prompt: str) -> LLMResponse:
        """Send a prompt to the LLM and return the response."""
        response = self._client.chat.completions.create(
            model=self.model,
            messages=[
                {"role": "system", "content": system_prompt},
                {"role": "user", "content": user_prompt},
            ],
            temperature=0.3,
            max_tokens=8192,
        )

        choice = response.choices[0]
        usage = response.usage

        prompt_tokens = usage.prompt_tokens if usage else 0
        completion_tokens = usage.completion_tokens if usage else 0

        self.total_prompt_tokens += prompt_tokens
        self.total_completion_tokens += completion_tokens

        return LLMResponse(
            text=choice.message.content or "",
            prompt_tokens=prompt_tokens,
            completion_tokens=completion_tokens,
            model=self.model,
        )
