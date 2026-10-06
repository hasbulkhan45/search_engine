import pytest
import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))
try:
    import search_engine_native as se
except ImportError:
    se = None

from crawler.storage import DocumentStore

def test_full_e2e_crawl_index_search():
    assert se is not None, "C++ native extension must be compiled"

    index = se.Index()
    tokenizer = se.Tokenizer()
    store = DocumentStore(db_path=":memory:")

    # 1. Simulate crawling documents
    doc_id = store.save_document(
        url="https://antigravity.io/docs",
        domain="antigravity.io",
        title="Antigravity Architecture Guide",
        body="High performance search engine written in C++17 with Python bindings and BM25 ranker.",
        meta={"framework": "antigravity"}
    )

    # 2. Add to C++ index
    doc = se.Document(doc_id, "https://antigravity.io/docs", "Antigravity Architecture Guide", "High performance search engine written in C++17 with Python bindings and BM25 ranker.")
    tokens = tokenizer.tokenize(doc.full_text())
    index.add_document(doc, tokens)

    assert index.doc_count() == 1

    # 3. Query using BM25
    scorer = se.BM25Scorer(index)
    results = scorer.score(tokenizer.tokenize("C++17 search engine"))

    assert len(results) > 0
    assert results[0].doc_id == doc_id
    assert results[0].score > 0.0

    # 4. Generate snippet
    snip_gen = se.SnippetGenerator(max_snippet_length=80)
    snip = snip_gen.generate(doc.body, ["c++17", "search"])
    assert snip.has_match is True
