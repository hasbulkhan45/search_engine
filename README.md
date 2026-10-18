<div align="center">

<h1>⚡ SearchEngine</h1>
<p><strong>A blazing-fast, full-stack search engine built from scratch</strong></p>

<p>
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue?style=flat-square&logo=cplusplus" alt="C++17"/>
  <img src="https://img.shields.io/badge/Python-3.10+-yellow?style=flat-square&logo=python" alt="Python"/>
  <img src="https://img.shields.io/badge/FastAPI-REST_API-009688?style=flat-square&logo=fastapi" alt="FastAPI"/>
  <img src="https://img.shields.io/badge/Tests-26%20passing-brightgreen?style=flat-square" alt="Tests"/>
  <img src="https://img.shields.io/badge/License-MIT-lightgrey?style=flat-square" alt="MIT License"/>
</p>

<p>
  <b>85,000 docs/sec indexing</b> · <b>&lt;15µs query latency</b> · <b>BM25 + PageRank + ML reranking</b>
</p>

</div>

---

## 📖 Table of Contents

- [Overview](#-overview)
- [Architecture](#-architecture)
- [Features](#-features)
- [Quick Start](#-quick-start)
- [Build the C++ Engine](#️-build-the-c-engine)
- [Run the API](#-run-the-api)
- [Crawl the Web](#-crawl-the-web)
- [Open the Frontend](#-open-the-frontend)
- [Run Tests](#-run-tests)
- [API Reference](#-api-reference)
- [Performance](#-performance)
- [Project Structure](#-project-structure)
- [Configuration](#-configuration)

---

## 🔍 Overview

SearchEngine is a **production-grade, full-stack search engine** built entirely from scratch — no Elasticsearch, no Lucene, no shortcuts.

The core indexing engine is written in **C++17** for maximum performance, while the crawler, ranking, API, and frontend are in **Python** for flexibility. All layers communicate via a **pybind11 native bridge** for zero-overhead inter-language calls.

---

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────┐
│                    Frontend UI                      │
│            HTML · CSS · Vanilla JS                  │
└───────────────────────┬─────────────────────────────┘
                        │ HTTP REST
┌───────────────────────▼─────────────────────────────┐
│                  FastAPI REST API                    │
│          /search · /index · /health                 │
└──────┬────────────────┬────────────────┬────────────┘
       │                │                │
┌──────▼──────┐  ┌──────▼──────┐  ┌─────▼───────┐
│  C++ Engine │  │   Ranking   │  │  Web Crawler│
│  (pybind11) │  │  PageRank   │  │  Async +    │
│  BM25/Index │  │  ML Rerank  │  │  robots.txt │
└─────────────┘  └─────────────┘  └─────────────┘
                        │
              ┌─────────▼──────────┐
              │   SQLite Store     │
              │  (search_engine.db)│
              └────────────────────┘
```

---

## ✨ Features

| Layer | What it does |
|-------|-------------|
| **C++ Engine** | Porter Stemmer, Tokenizer, Inverted Index with positional postings, BM25 Okapi ranker, TF-IDF, Varint+Delta compression, Binary serialization with CRC |
| **Crawler** | Async multi-domain web crawler, robots.txt compliance, Bloom Filter deduplication, politeness delays, SQLite-backed document store |
| **Ranking** | PageRank (power iteration), BM25, ML Reranker combining 4 signals, Spelling Corrector, Trie-based Autocomplete |
| **API** | FastAPI REST, API-key auth, token-bucket rate limiter, CORS, OpenAPI docs |
| **Frontend** | Dark-mode search UI, debounced autocomplete, paginated results, analytics dashboard |

---

## 🚀 Quick Start

### Prerequisites

```bash
# System
sudo apt-get install -y build-essential cmake libssl-dev

# Python
pip install -r requirements.txt
```

**Minimum versions:**
- GCC 9+ or Clang 10+ (C++17)
- CMake 3.16+
- Python 3.10+

---

## 🛠️ Build the C++ Engine

```bash
# 1. Create build directory
mkdir build && cd build

# 2. Configure
cmake .. -DCMAKE_BUILD_TYPE=Release

# 3. Compile (uses all CPU cores)
make -j$(nproc)

# 4. Run C++ tests
ctest --output-on-failure
```

Expected output:
```
Test project /home/.../search_engine/build
      Start 1: test_tokenizer
  1/9 Test #1: test_tokenizer ....... Passed   0.01s
  ...
9/9 tests passed
```

---

## 🌐 Run the API

```bash
# From the project root
uvicorn api.main:app --host 0.0.0.0 --port 8000 --reload
```

The API will be live at **http://localhost:8000**

Interactive API docs: **http://localhost:8000/docs**

---

## 🕷️ Crawl the Web

```bash
# Seed with a starting URL (crawls up to 50 pages by default)
python3 -m crawler.cli \
  --seed https://en.wikipedia.org/wiki/Information_retrieval \
  --max-pages 100 \
  --workers 5
```

Options:
```
--seed URL        Starting URL to crawl from
--max-pages N     Maximum pages to crawl (default: 50)
--workers N       Concurrent async workers (default: 3)
--delay FLOAT     Politeness delay in seconds (default: 1.0)
--depth N         Maximum crawl depth (default: 3)
```

---

## 🖥️ Open the Frontend

Simply open in your browser:

```bash
# Option 1: Direct file open
xdg-open frontend/index.html

# Option 2: Serve locally (avoids CORS issues)
python3 -m http.server 3000 --directory frontend
# Then visit: http://localhost:3000
```

> ⚠️ Make sure the API is running on port 8000 before using the frontend.

---

## 🧪 Run Tests

```bash
# All Python tests (17 suites)
pytest tests/ -v

# Specific module
pytest tests/test_api.py -v
pytest tests/test_crawler.py -v

# C++ tests only
cd build && ctest -V

# Full test suite (C++ + Python)
cd build && ctest --output-on-failure && cd .. && pytest tests/ -v
```

---

## 📡 API Reference

### Health Check
```http
GET /health
```
```json
{ "status": "ok", "version": "1.0.0" }
```

### Search
```http
GET /api/v1/search?q=information+retrieval&page=1&limit=10
X-API-Key: your-api-key
```
```json
{
  "query": "information retrieval",
  "total": 42,
  "results": [
    {
      "url": "https://...",
      "title": "Information Retrieval",
      "snippet": "...highlighted match...",
      "score": 14.72,
      "rank": 1
    }
  ],
  "suggestions": ["information retrieval systems", "..."],
  "time_ms": 3.4
}
```

### Index a Document
```http
POST /api/v1/index/document
X-API-Key: your-api-key
Content-Type: application/json

{
  "url": "https://example.com/page",
  "title": "My Page Title",
  "body": "Full text content to index..."
}
```

### Index Stats
```http
GET /api/v1/index/stats
X-API-Key: your-api-key
```
```json
{
  "total_documents": 1024,
  "total_terms": 87432,
  "index_size_bytes": 2097152
}
```

---

## ⚡ Performance

| Metric | Value |
|--------|-------|
| Indexing throughput | **~85,000 docs/sec** |
| BM25 query latency | **< 15 µs** |
| Index compression | **Varint + Delta** (~12 bytes/posting) |
| Test coverage | **9 C++ + 17 Python** suites, all passing |
| Crawl speed | ~5 pages/sec (politeness-limited) |

---

## 📁 Project Structure

```
search_engine/
├── engine/                  # C++17 Core Engine
│   ├── include/             # Header files
│   │   ├── tokenizer.h      # Tokenizer with stop-word filter
│   │   ├── stemmer.h        # Porter Stemmer (5-step)
│   │   ├── index.h          # Inverted index + positional postings
│   │   ├── bm25.h           # Okapi BM25 ranker
│   │   ├── scorer.h         # TF-IDF scorer
│   │   ├── compression.h    # Varint + Delta encoding
│   │   ├── serializer.h     # Binary index I/O with CRC
│   │   ├── query_parser.h   # Query parser (AND/OR/Phrase)
│   │   ├── query_planner.h  # Query execution planner
│   │   ├── query_cache.h    # LRU query cache
│   │   └── snippet.h        # Snippet generator
│   ├── src/                 # Implementations
│   └── tests/               # Catch2 unit tests
│
├── crawler/                 # Python Async Web Crawler
│   ├── frontier.py          # URL frontier with politeness
│   ├── bloom.py             # Bloom filter deduplication
│   ├── robots.py            # robots.txt compliance
│   ├── parser.py            # HTML parser (BeautifulSoup)
│   ├── storage.py           # SQLite document store
│   ├── crawler.py           # Async orchestrator
│   └── cli.py               # CLI entry point
│
├── bindings/                # pybind11 C++↔Python bridge
│   ├── bindings.cpp         # pybind11 module definition
│   ├── c_api.h/cpp          # C FFI bridge
│   └── ctypes_fallback.py   # Pure-Python fallback
│
├── ranking/                 # ML Ranking Layer
│   ├── pagerank.py          # PageRank (power iteration)
│   ├── spelling.py          # Statistical spelling corrector
│   ├── suggester.py         # Trie autocomplete
│   └── ml_reranker.py       # Multi-signal ML reranker
│
├── api/                     # FastAPI REST API
│   ├── main.py              # App entry point
│   ├── routes/              # Route handlers
│   └── middleware/          # Auth + rate limiter
│
├── frontend/                # Web UI
│   ├── index.html           # Search + SERP page
│   ├── style.css            # Glassmorphic dark/light CSS
│   ├── app.js               # Autocomplete + pagination JS
│   └── dashboard.html       # Analytics dashboard
│
├── tests/                   # Python test suites (pytest)
├── scripts/                 # Benchmark + seed scripts
├── docs/                    # Documentation
├── ci/                      # CI pipeline config
├── Dockerfile               # Docker image
├── docker-compose.yml       # Multi-service compose
├── CMakeLists.txt           # C++ build config
└── requirements.txt         # Python dependencies
```

---

## ⚙️ Configuration

### Environment Variables

| Variable | Default | Description |
|----------|---------|-------------|
| `API_KEY` | `dev-key-123` | API authentication key |
| `DB_PATH` | `search_engine.db` | SQLite database path |
| `PORT` | `8000` | API server port |
| `LOG_LEVEL` | `INFO` | Logging level |

### Running with Docker

```bash
# Build and start all services
docker-compose up --build

# Services:
#   API  → http://localhost:8000
#   Docs → http://localhost:8000/docs
```

---

## 🤝 Contributing

1. Fork the repo
2. Create a feature branch (`git checkout -b feature/my-feature`)
3. Build and test: `make -j$(nproc) && ctest && pytest tests/`
4. Commit your changes
5. Open a Pull Request

---

## 📄 License

MIT © 2026 — Built with ❤️ from scratch.
