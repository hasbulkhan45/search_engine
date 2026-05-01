# Architecture Notes
[2026-02-20] Engine: Tokenizer → Stemmer → Index pipeline
[2026-02-22] Bindings layer: pybind11 wraps C++ Index class
[2026-02-24] Scoring: BM25 primary + TF-IDF secondary signal
[2026-02-27] Storage: binary index + SQLite doc store
[2026-03-03] Snippet: boundary-aware sentence extraction
[2026-03-07] Fields: title(f=0), body(f=1), url(f=2)
[2026-03-11] Bindings: pybind11 primary, ctypes fallback secondary
[2026-03-15] Phase 1 done: Tokenizer+Stemmer+Index+BM25+Serializer
[2026-03-23] Storage: segments → merge → compacted index
[2026-03-27] DiskIndex: file-backed with LRU decode cache
[2026-03-31] Phase 2 done: serializer+DiskIndex+FieldScorer+Vacuum
[2026-04-15] Phase 3 done: frontier+bloom+robots+parser+storage+cli
[2026-04-21] Phase 4 done: pybind11+c_api+ctypes+stubs
[2026-05-01] Phase 5 done: REST API+auth+rate-limit+OpenAPI
