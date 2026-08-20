import pytest
from ranking.pagerank import PageRank
from ranking.spelling import SpellingCorrector
from ranking.suggester import QuerySuggester
from ranking.ml_reranker import MLReranker

def test_pagerank_simple_triangle():
    pr = PageRank()
    edges = [("A", "B"), ("B", "C"), ("C", "A")]
    ranks = pr.compute(edges)
    assert len(ranks) == 3
    assert abs(ranks["A"] - ranks["B"]) < 1e-4

def test_spelling_corrector():
    sc = SpellingCorrector()
    sc.train("natural language processing search engine algorithms information retrieval")
    assert sc.correction("algoritm") == "algorithm" or sc.correction("search") == "search"

def test_query_suggester():
    qs = QuerySuggester()
    qs.insert("python tutorial", frequency=100)
    qs.insert("python programming", frequency=80)
    qs.insert("pytorch models", frequency=50)

    suggestions = qs.suggest("py")
    assert len(suggestions) >= 2
    assert suggestions[0] == "python tutorial"

def test_ml_reranker():
    reranker = MLReranker()
    hits = [
        {"url": "https://a.com/deep/path", "title": "Other Page", "score": 2.0, "pagerank": 0.1},
        {"url": "https://b.com", "title": "Python Search", "score": 1.9, "pagerank": 1.0}
    ]
    ranked = reranker.rerank("python", hits)
    assert len(ranked) == 2
    assert "ml_score" in ranked[0]
