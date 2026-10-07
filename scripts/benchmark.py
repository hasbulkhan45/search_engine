#!/usr/bin/env python3
import time
import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))
import search_engine_native as se

def run_benchmarks():
    print("=" * 60)
    print(" ANTIGRAVITY SEARCH ENGINE - PERFORMANCE BENCHMARK")
    print("=" * 60)

    index = se.Index()
    tokenizer = se.Tokenizer()

    N_DOCS = 10000
    sample_text = "The quick brown fox jumps over the lazy dog. Modern information retrieval systems utilize inverted indexes and BM25 probabilistic models for ultra low latency document ranking."

    print(f"Indexing {N_DOCS} synthetic documents into C++ engine...")
    t0 = time.perf_counter()
    tokens = tokenizer.tokenize(sample_text)
    for i in range(1, N_DOCS + 1):
        doc = se.Document(i, f"https://example.com/doc/{i}", f"Document Title {i}", sample_text)
        index.add_document(doc, tokens)
    t1 = time.perf_counter()
    index_time = t1 - t0
    docs_per_sec = N_DOCS / index_time

    print(f"-> Indexed {N_DOCS} documents in {index_time:.3f}s ({docs_per_sec:.0f} docs/sec)")
    print(f"-> In-memory vocabulary size: {index.vocab_size()} terms")

    scorer = se.BM25Scorer(index)
    q_tokens = tokenizer.tokenize("inverted index BM25")

    N_QUERIES = 5000
    print(f"Running {N_QUERIES} BM25 search queries...")
    t2 = time.perf_counter()
    for _ in range(N_QUERIES):
        res = scorer.score(q_tokens)
    t3 = time.perf_counter()

    query_time = t3 - t2
    qps = N_QUERIES / query_time
    latency_us = (query_time / N_QUERIES) * 1e6

    print(f"-> Executed {N_QUERIES} queries in {query_time:.3f}s")
    print(f"-> Throughput: {qps:.0f} QPS")
    print(f"-> Average Latency: {latency_us:.1f} µs/query")
    print("=" * 60)

if __name__ == "__main__":
    run_benchmarks()
