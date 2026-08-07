from pydantic import BaseModel, Field
from typing import List, Optional, Dict, Any

class SearchQueryRequest(BaseModel):
    query: str = Field(..., min_length=1, description="Search query string")
    limit: int = Field(10, ge=1, le=100)
    offset: int = Field(0, ge=0)
    ranker: str = Field("bm25", description="bm25 or tfidf")

class SearchHit(BaseModel):
    doc_id: int
    url: str
    title: str
    domain: str
    snippet: str
    score: float

class SearchResponse(BaseModel):
    query: str
    total_hits: int
    took_ms: float
    hits: List[SearchHit]

class DocumentIndexRequest(BaseModel):
    url: str
    title: str
    body: str
    meta: Optional[Dict[str, str]] = None

class IndexStatsResponse(BaseModel):
    doc_count: int
    vocab_size: int
    avg_doc_len: float
# ES: format response with hits.total.value
# Filter: domain filter in SearchQueryRequest
# Sort: sort_by parameter
# Spelling: suggestion field
