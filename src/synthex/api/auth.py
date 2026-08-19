"""API key authentication and rate limiting."""

from __future__ import annotations

import hashlib
import json
import os
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from fastapi import HTTPException, Security
from fastapi.security import APIKeyHeader


API_KEY_HEADER = APIKeyHeader(name="X-API-Key")

# Storage path for API keys and usage
DATA_DIR = Path(os.environ.get("SYNTHEX_DATA_DIR", "data"))
KEYS_FILE = DATA_DIR / "api_keys.json"
USAGE_FILE = DATA_DIR / "usage.json"


@dataclass
class Tier:
    name: str
    requests_per_day: int
    max_code_bytes: int
    generations: int  # max pipeline generations allowed
    price_monthly: float  # USD


TIERS: dict[str, Tier] = {
    "free": Tier(
        name="free",
        requests_per_day=10,
        max_code_bytes=50_000,
        generations=1,
        price_monthly=0,
    ),
    "pro": Tier(
        name="pro",
        requests_per_day=200,
        max_code_bytes=500_000,
        generations=3,
        price_monthly=49,
    ),
    "team": Tier(
        name="team",
        requests_per_day=2000,
        max_code_bytes=2_000_000,
        generations=3,
        price_monthly=199,
    ),
}


@dataclass
class APIKey:
    key_hash: str
    tier: str
    owner: str
    created_at: str
    active: bool = True


@dataclass
class UsageRecord:
    key_hash: str
    date: str
    requests: int = 0
    tokens_consumed: int = 0
    bytes_processed: int = 0


def _hash_key(key: str) -> str:
    return hashlib.sha256(key.encode()).hexdigest()


def _load_keys() -> dict[str, dict]:
    if KEYS_FILE.exists():
        return json.loads(KEYS_FILE.read_text(encoding="utf-8"))
    return {}


def _save_keys(keys: dict) -> None:
    DATA_DIR.mkdir(parents=True, exist_ok=True)
    KEYS_FILE.write_text(json.dumps(keys, indent=2), encoding="utf-8")


def _load_usage() -> dict[str, dict]:
    if USAGE_FILE.exists():
        return json.loads(USAGE_FILE.read_text(encoding="utf-8"))
    return {}


def _save_usage(usage: dict) -> None:
    DATA_DIR.mkdir(parents=True, exist_ok=True)
    USAGE_FILE.write_text(json.dumps(usage, indent=2), encoding="utf-8")


def create_api_key(owner: str, tier: str = "free") -> str:
    """Create a new API key. Returns the raw key (only shown once)."""
    import secrets
    raw_key = f"sx-{tier}-{secrets.token_hex(24)}"
    key_hash = _hash_key(raw_key)

    keys = _load_keys()
    keys[key_hash] = {
        "tier": tier,
        "owner": owner,
        "created_at": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
        "active": True,
    }
    _save_keys(keys)
    return raw_key


def validate_key(api_key: str = Security(API_KEY_HEADER)) -> dict:
    """FastAPI dependency: validate API key, check rate limit, return key info."""
    key_hash = _hash_key(api_key)
    keys = _load_keys()

    if key_hash not in keys:
        raise HTTPException(status_code=401, detail="Invalid API key")

    key_data = keys[key_hash]
    if not key_data.get("active", True):
        raise HTTPException(status_code=403, detail="API key deactivated")

    tier_name = key_data.get("tier", "free")
    tier = TIERS.get(tier_name, TIERS["free"])

    # Check rate limit
    today = time.strftime("%Y-%m-%d", time.gmtime())
    usage = _load_usage()
    usage_key = f"{key_hash}:{today}"
    day_usage = usage.get(usage_key, {"requests": 0, "tokens": 0, "bytes": 0})

    if day_usage["requests"] >= tier.requests_per_day:
        raise HTTPException(
            status_code=429,
            detail=f"Rate limit exceeded: {tier.requests_per_day} requests/day on {tier_name} tier",
        )

    return {
        "key_hash": key_hash,
        "tier": tier_name,
        "owner": key_data["owner"],
        "tier_config": tier,
        "usage_today": day_usage,
    }


def record_usage(key_hash: str, tokens: int = 0, bytes_processed: int = 0) -> None:
    """Record a request against the API key's daily usage."""
    today = time.strftime("%Y-%m-%d", time.gmtime())
    usage = _load_usage()
    usage_key = f"{key_hash}:{today}"

    if usage_key not in usage:
        usage[usage_key] = {"requests": 0, "tokens": 0, "bytes": 0}

    usage[usage_key]["requests"] += 1
    usage[usage_key]["tokens"] += tokens
    usage[usage_key]["bytes"] += bytes_processed
    _save_usage(usage)
