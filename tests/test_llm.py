"""Tests for synthex.llm — data structures only."""

from synthex.llm import LLMResponse


class TestLLMResponse:
    def test_total_tokens(self):
        r = LLMResponse(
            text="hello",
            prompt_tokens=100,
            completion_tokens=200,
            model="test",
        )
        assert r.total_tokens == 300

    def test_zero_tokens(self):
        r = LLMResponse(text="", prompt_tokens=0, completion_tokens=0, model="test")
        assert r.total_tokens == 0

    def test_fields(self):
        r = LLMResponse(
            text="code here",
            prompt_tokens=50,
            completion_tokens=150,
            model="gpt-4o-mini",
        )
        assert r.text == "code here"
        assert r.model == "gpt-4o-mini"
