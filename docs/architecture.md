# Search Engine System Architecture

## Overview
This search engine is designed with a hybrid language approach:
- **C++17 Core Engine**: High-performance text tokenization, inverted indexing, positional posting lists, and BM25 scoring.
- **Python Ecosystem**: Web crawling (HTTP fetching, robots.txt, HTML parsing), Machine Learning reranking (PageRank), REST API (FastAPI), and tests.
- **Modern Web Frontend**: Clean, responsive glassmorphism UI with real-time autocomplete and analytics.

## Component Breakdown

```
+-------------------------------------------------------------+
|                        Frontend UI                          |
|         (Vanilla JS, Glassmorphic CSS, Autocomplete)        |
+------------------------------+------------------------------+
                               | HTTP REST
+------------------------------v------------------------------+
|                    FastAPI Backend Service                  |
|          (/api/v1/search, /api/v1/index, /api/v1/crawler)   |
+--------------+-------------------------------+--------------+
               |                               |
+--------------v---------------+ +-------------v--------------+
|    Ranking & ML Layer        | |    Web Crawler Subsystem    |
| (PageRank, ML Reranker)      | | (URLFrontier, BloomFilter)  |
+--------------+---------------+ +-------------+--------------+
               |                               |
+--------------v-------------------------------v--------------+
|                pybind11 Native Python Bindings              |
+------------------------------+------------------------------+
                               |
+------------------------------v------------------------------+
|                      C++17 Engine Core                      |
| (Tokenizer, InvertedIndex, BM25, TF-IDF, Serializer, Snippet)|
+-------------------------------------------------------------+
```

## Storage Formats
1. **In-Memory Postings**: Compact lists of `(doc_id, freq, positions[])` with delta encoding.
2. **Binary Segments**: Disk persistence with 16-byte magic header, CRC32 checksum, and metadata.
3. **SQLite Document Store**: WAL-mode database storing raw crawled content, metadata, and link graph.
