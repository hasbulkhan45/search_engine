import pytest
import sys
import os

# Add root directory to python path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

import search_engine_native as se

def test_tokenizer_and_stemmer():
    tok = se.Tokenizer()
    tokens = tok.tokenize("The quick brown fox jumps over the lazy dog!")
    # 'the' is stopword
    assert "the" not in tokens
    assert "jump" in tokens or "jumps" in tokens
    assert tok.is_stopword("the") is True
    assert tok.is_stopword("algorithm") is False

def test_document_model():
    doc = se.Document(1, "https://wikipedia.org/wiki/Search_engine", "Search Engine", "An information retrieval software system.")
    assert doc.id == 1
    assert doc.domain == "wikipedia.org"
    assert doc.is_valid() is True
    doc.set_meta("author", "Antigravity")
    assert doc.get_meta("author") == "Antigravity"
    assert "Search Engine" in doc.full_text()

def test_index_and_bm25_scoring():
    idx = se.Index()
    tok = se.Tokenizer()

    d1 = se.Document(1, "https://example.com/ai", "Artificial Intelligence", "Deep learning neural networks and natural language processing.")
    d2 = se.Document(2, "https://example.com/db", "Database Systems", "Relational databases SQL transactions and indexing.")

    idx.add_document(d1, tok.tokenize(d1.full_text()))
    idx.add_document(d2, tok.tokenize(d2.full_text()))

    assert idx.doc_count() == 2
    assert idx.vocab_size() > 0

    ranker = se.BM25Scorer(idx)
    q_tokens = tok.tokenize("neural networks")
    results = ranker.score(q_tokens)

    assert len(results) > 0
    assert results[0].doc_id == 1
    assert results[0].score > 0.0

def test_query_parser_and_phrase_search():
    parser = se.QueryParser()
    pq = parser.parse('"deep learning"')
    assert pq.is_phrase is True
    assert pq.phrase == "deep learning"
    assert "deep" in pq.terms
    assert "learning" in pq.terms

    idx = se.Index()
    tok = se.Tokenizer()
    d = se.Document(1, "https://ml.org", "ML Title", "Introduction to deep learning algorithms")
    idx.add_document(d, tok.tokenize(d.full_text()))

    matches = idx.phrase_search(tok.tokenize("deep learning"))
    assert len(matches) == 1
    assert matches[0].doc_id == 1

def test_snippet_generator():
    gen = se.SnippetGenerator(max_snippet_length=80)
    body = "Antigravity search engine core is written in C++17 with blazing fast inverted index structures."
    snip = gen.generate(body, ["blazing", "fast"])
    assert snip.has_match is True
    assert "blazing" in snip.text
# Test: ctypes C fallback lifecycle
# Test: multi-threaded native indexing
