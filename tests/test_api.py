import pytest
from fastapi.testclient import TestClient
from api.main import app

client = TestClient(app)

def test_health_endpoint():
    resp = client.get("/health")
    assert resp.status_code == 200
    assert resp.json()["status"] == "ok"

def test_search_endpoint_empty():
    resp = client.get("/api/v1/search?q=algorithms")
    assert resp.status_code == 200
    data = resp.json()
    assert "query" in data
    assert "took_ms" in data
    assert "hits" in data

def test_index_stats_endpoint():
    resp = client.get("/api/v1/index/stats")
    assert resp.status_code == 200
    data = resp.json()
    assert "doc_count" in data
    assert "vocab_size" in data
