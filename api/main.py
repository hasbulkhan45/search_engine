from fastapi import FastAPI, Depends, Query
from fastapi.middleware.cors import CORSMiddleware
import time
import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))
try:
    import search_engine_native as se
except ImportError:
    se = None

from .config import settings
from .models import SearchResponse, SearchHit, IndexStatsResponse, DocumentIndexRequest
from .middleware.auth import verify_api_key
from crawler.storage import DocumentStore

app = FastAPI(title=settings.app_name, version="1.0.0")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Global engine state
native_index = se.Index() if se else None
tokenizer = se.Tokenizer() if se else None
doc_store = DocumentStore(db_path=settings.db_path)
snippet_gen = se.SnippetGenerator(max_snippet_length=140) if se else None

@app.get("/health")
def health_check():
    return {"status": "ok", "engine": "cpp17" if se else "python_fallback"}

@app.get("/api/v1/search", response_model=SearchResponse)
def search_documents(
    q: str = Query(..., min_length=1),
    limit: int = Query(10, ge=1, le=100),
    offset: int = Query(0, ge=0),
    ranker: str = Query("bm25")
):
    start = time.perf_counter()
    if not native_index or not tokenizer:
        return SearchResponse(query=q, total_hits=0, took_ms=0.0, hits=[])

    parser = se.QueryParser()
    parsed = parser.parse(q)
    terms = tokenizer.tokenize(q)

    if ranker == "tfidf":
        scorer = se.TFIDFScorer(native_index)
        results = scorer.score(terms)
    else:
        scorer = se.BM25Scorer(native_index)
        results = scorer.score(terms)

    total_hits = len(results)
    sliced = results[offset : offset + limit]

    hits = []
    for r in sliced:
        doc = native_index.get_doc(r.doc_id)
        if doc:
            snip = snippet_gen.generate(doc.body, terms) if snippet_gen else None
            hits.append(SearchHit(
                doc_id=doc.id,
                url=doc.url,
                title=doc.title,
                domain=doc.domain,
                snippet=snip.text if snip else doc.body[:120],
                score=round(r.score, 4)
            ))

    took_ms = round((time.perf_counter() - start) * 1000.0, 2)
    return SearchResponse(query=q, total_hits=total_hits, took_ms=took_ms, hits=hits)

@app.get("/api/v1/index/stats", response_model=IndexStatsResponse)
def index_stats():
    if not native_index:
        return IndexStatsResponse(doc_count=0, vocab_size=0, avg_doc_len=0.0)
    return IndexStatsResponse(
        doc_count=native_index.doc_count(),
        vocab_size=native_index.vocab_size(),
        avg_doc_len=round(native_index.avg_doc_len(), 2)
    )

@app.post("/api/v1/index/document", dependencies=[Depends(verify_api_key)])
def index_single_document(req: DocumentIndexRequest):
    if not native_index or not tokenizer:
        return {"status": "error", "message": "Engine not initialized"}
    
    doc_id = doc_store.save_document(
        url=req.url,
        domain=req.url.split("//")[-1].split("/")[0],
        title=req.title,
        body=req.body,
        meta=req.meta
    )
    doc = se.Document(doc_id, req.url, req.title, req.body)
    tokens = tokenizer.tokenize(doc.full_text())
    native_index.add_document(doc, tokens)
    return {"status": "indexed", "doc_id": doc_id}
# Tracing: assign UUID request_id
# OpenAPI: route tags and doc summaries
# Performance: GZipMiddleware minimum_size=500
# Crawler: background task trigger
# Crawler: live page count and queue depth
# Error: clean JSON format for 422 errors
# Metrics: prometheus request counters
# Cache-Control: max-age=60
