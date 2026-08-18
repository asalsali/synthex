"""Tests for synthex.benchmark — data structures only, no LLM calls."""

from synthex.benchmark import ArmResult, BenchmarkResult


class TestArmResult:
    def test_fields(self):
        arm = ArmResult(
            name="experimental",
            input_score=20,
            final_score=70,
            delta=50,
            compile_rate=0.75,
            scores_per_stage=[20, 60, 70],
            total_tokens=5000,
            duration_seconds=10.5,
        )
        assert arm.delta == 50
        assert arm.compile_rate == 0.75
        assert len(arm.scores_per_stage) == 3

    def test_zero_delta(self):
        arm = ArmResult(
            name="control",
            input_score=50,
            final_score=50,
            delta=0,
            compile_rate=0.0,
        )
        assert arm.delta == 0


class TestBenchmarkResult:
    def test_advantage(self):
        exp = ArmResult(name="exp", input_score=20, final_score=70, delta=50, compile_rate=0.5)
        ctrl = ArmResult(name="ctrl", input_score=20, final_score=40, delta=20, compile_rate=0.0)
        result = BenchmarkResult(
            function_name="test_func",
            input_score=20,
            experimental=exp,
            control=ctrl,
            advantage=30,
            compile_advantage=0.5,
        )
        assert result.advantage == 30
        assert result.compile_advantage == 0.5
        assert result.function_name == "test_func"

    def test_negative_advantage(self):
        exp = ArmResult(name="exp", input_score=50, final_score=55, delta=5, compile_rate=0.0)
        ctrl = ArmResult(name="ctrl", input_score=50, final_score=65, delta=15, compile_rate=0.5)
        result = BenchmarkResult(
            function_name="loss",
            input_score=50,
            experimental=exp,
            control=ctrl,
            advantage=-10,
            compile_advantage=-0.5,
        )
        assert result.advantage < 0
