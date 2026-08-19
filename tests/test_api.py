"""Tests for the Synthex API — auth, models, endpoints."""

import json
import os
import tempfile
from pathlib import Path
from unittest.mock import patch

import pytest

# Skip all tests if fastapi not installed
fastapi = pytest.importorskip("fastapi")

from synthex.api.auth import (
    create_api_key,
    _hash_key,
    _load_keys,
    TIERS,
)
from synthex.api.models import (
    RefineRequest,
    ScoreRequest,
    TypesRequest,
    ScoreResult,
)


@pytest.fixture
def tmp_data_dir(tmp_path):
    """Set up a temp data dir for API key storage."""
    with patch.dict(os.environ, {"SYNTHEX_DATA_DIR": str(tmp_path)}):
        import synthex.api.auth as auth_mod
        auth_mod.DATA_DIR = tmp_path
        auth_mod.KEYS_FILE = tmp_path / "api_keys.json"
        auth_mod.USAGE_FILE = tmp_path / "usage.json"
        yield tmp_path


class TestAuth:
    def test_create_key(self, tmp_data_dir):
        key = create_api_key("test_user", "free")
        assert key.startswith("sx-free-")
        assert len(key) > 20

    def test_create_key_pro(self, tmp_data_dir):
        key = create_api_key("pro_user", "pro")
        assert key.startswith("sx-pro-")

    def test_key_stored(self, tmp_data_dir):
        key = create_api_key("stored_user", "free")
        key_hash = _hash_key(key)
        keys = json.loads((tmp_data_dir / "api_keys.json").read_text())
        assert key_hash in keys
        assert keys[key_hash]["owner"] == "stored_user"
        assert keys[key_hash]["tier"] == "free"

    def test_hash_deterministic(self):
        assert _hash_key("test") == _hash_key("test")
        assert _hash_key("a") != _hash_key("b")


class TestTiers:
    def test_free_tier(self):
        t = TIERS["free"]
        assert t.requests_per_day == 10
        assert t.generations == 1
        assert t.price_monthly == 0

    def test_pro_tier(self):
        t = TIERS["pro"]
        assert t.requests_per_day == 200
        assert t.generations == 3
        assert t.price_monthly == 49

    def test_team_tier(self):
        t = TIERS["team"]
        assert t.requests_per_day == 2000
        assert t.price_monthly == 199


class TestModels:
    def test_refine_request(self):
        r = RefineRequest(code="int x;", generations=2, model="gpt-4o-mini")
        assert r.code == "int x;"
        assert r.generations == 2

    def test_refine_request_defaults(self):
        r = RefineRequest(code="int x;")
        assert r.generations == 3
        assert r.model == "gpt-4o-mini"

    def test_score_request(self):
        r = ScoreRequest(code="int x = 0;")
        assert r.code == "int x = 0;"

    def test_types_request(self):
        r = TypesRequest(code="void *p = calloc(1, 0x208);")
        assert "calloc" in r.code

    def test_score_result(self):
        r = ScoreResult(
            total_score=61,
            lines=100,
            goto_count=0,
            meaningful_names=0.75,
            type_coverage=0.9,
            comment_density=0.1,
            cyclomatic_complexity=15,
            magic_number_count=0,
            compiles=True,
        )
        assert r.total_score == 61


class TestAPIEndpoints:
    """Test API endpoints using FastAPI TestClient."""

    @pytest.fixture
    def client(self, tmp_data_dir):
        from fastapi.testclient import TestClient
        from synthex.api.server import app
        return TestClient(app)

    @pytest.fixture
    def api_key(self, tmp_data_dir):
        return create_api_key("test_user", "free")

    def test_health(self, client):
        r = client.get("/health")
        assert r.status_code == 200
        assert r.json()["status"] == "ok"

    def test_score_endpoint(self, client, api_key):
        r = client.post(
            "/v1/score",
            json={"code": "int main() { return 0; }"},
            headers={"X-API-Key": api_key},
        )
        assert r.status_code == 200
        data = r.json()
        assert "score" in data
        assert data["score"]["total_score"] >= 0

    def test_types_endpoint(self, client, api_key):
        r = client.post(
            "/v1/types",
            json={"code": "void *p = calloc(1, 0x208); *(int*)(p + 0x200) = 1;"},
            headers={"X-API-Key": api_key},
        )
        assert r.status_code == 200
        data = r.json()
        assert "hints" in data
        assert "header" in data

    def test_usage_endpoint(self, client, api_key):
        r = client.get(
            "/v1/usage",
            headers={"X-API-Key": api_key},
        )
        assert r.status_code == 200
        data = r.json()
        assert data["tier"] == "free"

    def test_invalid_key(self, client):
        r = client.post(
            "/v1/score",
            json={"code": "int x;"},
            headers={"X-API-Key": "sx-fake-invalid"},
        )
        assert r.status_code == 401

    def test_missing_key(self, client):
        r = client.post("/v1/score", json={"code": "int x;"})
        assert r.status_code in (401, 403)  # Missing API key

    def test_score_updates_usage(self, client, api_key):
        # Make a request
        client.post(
            "/v1/score",
            json={"code": "int x;"},
            headers={"X-API-Key": api_key},
        )
        # Check usage incremented
        r = client.get("/v1/usage", headers={"X-API-Key": api_key})
        assert r.json()["requests_today"] >= 1
