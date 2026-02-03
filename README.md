# SearchEngine

A high-performance, full-stack search engine built from scratch using **C++** for the core engine and **Python** for the crawler, API, ranking, and frontend.

## Architecture

```
search_engine/
├── engine/        # C++17 — tokenizer, inverted index, BM25
├── crawler/       # Python — async web crawler
├── bindings/      # pybind11 — Python ↔ C++ bridge
├── api/           # FastAPI — REST API
├── ranking/       # Python+C++ — PageRank, ML reranking
├── frontend/      # HTML/CSS/JS — search UI
└── tests/         # pytest + catch2 tests
```

## Status: In Development (Phase 1 — Engine Core)

## Building the Engine (C++)

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

## Requirements

- C++17 compiler (GCC 9+ or Clang 10+)
- CMake 3.16+
- Python 3.10+
- pybind11

## License

MIT
