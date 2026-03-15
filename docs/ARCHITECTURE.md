# Architecture Notes
[2026-02-20] Engine: Tokenizer → Stemmer → Index pipeline
[2026-02-22] Bindings layer: pybind11 wraps C++ Index class
[2026-02-24] Scoring: BM25 primary + TF-IDF secondary signal
[2026-02-27] Storage: binary index + SQLite doc store
[2026-03-03] Snippet: boundary-aware sentence extraction
[2026-03-07] Fields: title(f=0), body(f=1), url(f=2)
[2026-03-11] Bindings: pybind11 primary, ctypes fallback secondary
[2026-03-15] Phase 1 done: Tokenizer+Stemmer+Index+BM25+Serializer
