# Changelog

All notable changes to the Antigravity Search Engine project are documented below.

## [1.0.0] - 2026-10-18
### Added
- High-performance C++17 Inverted Index with positional posting lists and delta compression.
- Porter Stemmer 5-step morphological reduction and stop words filtering.
- Okapi BM25 and TF-IDF probabilistic relevance scoring engines.
- Zero-copy string_view token streaming and binary serializer with CRC32 integrity check.
- Asynchronous Python web crawler with URLFrontier, BloomFilter deduplication, and robots.txt parsing.
- Native Python bindings via pybind11 with pure C fallback.
- FastAPI REST API service with /search, /index, and /crawler endpoints.
- PageRank graph analysis, ML feature-based reranker, spelling corrector, and prefix query suggester.
- Modern responsive search frontend UI with dark/light themes, instant autocomplete, and analytics dashboard.
- Catch2 and pytest test suites with 100% pass rate.
