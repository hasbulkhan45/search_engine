# Dev Notes
[2026-02-15] Porter stemmer handles edge cases for -ies suffix
[2026-02-15] Tokenizer ASCII table faster than std::tolower
[2026-02-16] Inverted index uses sorted postings for merge-joins
[2026-02-17] Stop word list: 127 English function words
[2026-02-17] Token frequency map preallocated at 512 buckets
[2026-02-18] Boolean AND uses bitset intersection approach
[2026-02-19] Phrase search uses positional postings windowing
[2026-02-19] SimHash fingerprint length = 64 bits
[2026-02-20] Unit tests for tokenizer cover 28 edge cases
[2026-02-21] CMake target: libsearch_engine static lib
[2026-02-21] CXX_STANDARD set to 17 globally in CMakeLists
[2026-02-22] Position-independent code flag needed for pybind11
[2026-02-23] BM25 k1=1.5, b=0.75 are default Okapi parameters
[2026-02-23] avgdl (average document length) updated incrementally
[2026-02-24] TF-IDF uses natural log for IDF calculation
[2026-02-25] Varint encoding: LEB128 format, 7 bits per byte
[2026-02-25] Delta encoding reduces posting list size by ~60%
[2026-02-26] Binary serializer writes 4-byte magic header: SEIDX
[2026-02-26] CRC32 checksum validates index integrity on load
[2026-02-27] DiskIndex maps term→file offset for lazy loading
[2026-02-28] FieldScorer weights: title=3.0, body=1.0, url=1.5
[2026-02-28] SegmentManager handles index merge and rotation
[2026-03-01] IndexVacuum removes deleted doc postings on GC
[2026-03-01] Query cache uses LRU eviction, default 256 entries
[2026-03-02] QueryParser handles quoted phrases with backslash escape
[2026-03-02] QueryPlanner chooses merge order by posting list size
[2026-03-03] Snippet generator uses 160-char window around query terms
[2026-03-04] All 9 Catch2 test suites pass with zero warnings
[2026-03-04] CTest registered: test_tokenizer through test_snippet
[2026-03-05] Engine benchmark: 85,001 docs/sec on 8-core machine
[2026-03-06] Tokenizer strips punctuation using ASCII bitmask
[2026-03-06] URL tokenizer splits on / . - _ for URL indexing
[2026-03-07] Multi-field index stores separate posting lists per field
[2026-03-08] Index merge: priority-queue based k-way merge
[2026-03-08] Segment files use .idx extension, metadata in .meta
[2026-03-09] CMake Release mode: -O3 -DNDEBUG -march=native
[2026-03-09] Debug mode adds -fsanitize=address,undefined
[2026-03-10] pybind11 module name: search_engine_native
[2026-03-10] .pyi stub file provides IDE type hints for bindings
[2026-03-11] ctypes_fallback.py provides pure-Python path if .so missing
[2026-03-12] C FFI bridge exposes flat C API for future wrappers
[2026-03-12] c_api.h uses extern C linkage for ABI stability
[2026-03-13] Engine header-only types in document.h: DocId=uint32_t
[2026-03-13] PostingEntry: {doc_id, positions, field_id, tf}
[2026-03-14] SimHash Hamming distance threshold = 3 for near-dups
[2026-03-14] Document fingerprint checked before indexing to skip dups
[2026-03-15] Phase 1 complete: all C++ engine components built & tested
[2026-03-16] Binary index format: [magic][version][term_count][...]
[2026-03-17] Index writer buffers 10K postings before flush
[2026-03-17] Index reader uses mmap for read-only fast access
[2026-03-19] BM25 idf floor at 0.1 to avoid negative scores
[2026-03-20] Posting list merged with skiplist for O(log n) seeks
[2026-03-20] Skiplist interval = sqrt(posting_list_length)
[2026-03-22] Multi-field scorer: sum(field_weight * field_bm25)
[2026-03-22] Field weights configurable at query time
[2026-03-23] Index merge uses external sort for large segment files
[2026-03-24] Vacuum marks deleted docs with tombstone byte
[2026-03-24] Post-vacuum, segments are merged and rewritten
[2026-03-25] Varint saves avg 2.1 bytes per posting vs int32
[2026-03-26] Binary serializer version field allows forward compat
[2026-03-26] Index load time for 1M docs: ~340ms from disk
[2026-03-27] DiskIndex caches last 64 decoded posting lists in LRU
[2026-03-28] SegmentManager caps segment size at 100K postings
[2026-03-28] Merge triggered when segment count exceeds 8
[2026-03-29] CMakeLists: add_library(search_engine STATIC ...)
[2026-03-29] All Phase 2 sources included in CMake target
[2026-03-30] BM25 scorer caches per-doc lengths for fast access
[2026-03-31] Phase 2 complete: full indexer+storage pipeline
[2026-04-01] Phase 3 begins: Python async web crawler
[2026-04-11] DocumentStore.add_document() returns new doc_id
[2026-04-11] SQLite FTS5 not used; custom engine handles full-text
[2026-04-14] Seed script populates 20 foundational CS/IR docs
[2026-04-14] Benchmark script measures indexing at 85,001 docs/sec
[2026-04-16] Phase 4 begins: Python↔C++ pybind11 bindings
[2026-04-16] pybind11 exposes: Index, Tokenizer, BM25Scorer
[2026-04-17] pybind11 module compiled with -fvisibility=hidden
[2026-04-17] Python GIL released during C++ index build
[2026-04-18] pybind11 buffer protocol exposes posting list as memoryview
[2026-04-18] .pyi stub regenerated via pybind11-stubgen
[2026-04-19] C FFI: search_engine_create() returns opaque handle
[2026-04-19] ctypes_fallback reimplements Index.search() in pure Python
[2026-04-20] Bindings test: import search_engine_native → Index()
[2026-04-20] CI tests both pybind11 path and ctypes fallback
[2026-04-21] Phase 4 complete: bindings + C FFI + ctypes fallback
[2026-05-02] Phase 6 begins: ML ranking layer
[2026-05-02] PageRank: power iteration converges at ~50 iterations
[2026-05-03] PageRank damping factor = 0.85 (Brin & Page default)
[2026-05-03] Dangling node mass redistributed uniformly each iteration
[2026-05-04] Spelling corrector: edit distance 1 candidates generated
[2026-05-04] Unigram model trained on indexed corpus frequencies
[2026-05-05] Autocomplete Trie: prefix search in O(k) where k=prefix len
[2026-05-05] Suggester returns top-5 completions by frequency rank
[2026-05-06] ML Reranker combines: BM25, PageRank, title_match, url_depth
[2026-05-06] Reranker weights: BM25=0.6, PageRank=0.2, title=0.15, url=0.05
[2026-05-07] PageRank scores normalized to [0, 1] before combining
[2026-05-08] Spelling correction only applied when query has no results
[2026-05-08] Did-you-mean suggestion shown if corrected query differs
[2026-05-09] Trie supports unicode via UTF-8 character boundaries
[2026-05-09] Autocomplete updates on each keystroke with 150ms debounce
[2026-05-10] Phase 6 complete: PageRank+spelling+autocomplete+ML rerank
[2026-06-11] Phase 7 begins: frontend UI
[2026-06-11] UI: dark/light mode CSS with CSS custom properties
[2026-06-12] Glassmorphism: backdrop-filter blur(12px) on result cards
[2026-06-12] Primary color: hsl(217, 91%, 60%) — bright blue
[2026-06-13] Search bar uses debounced input for autocomplete (150ms)
[2026-06-13] Autocomplete dropdown shows max 8 suggestions
[2026-06-14] Pagination: prev/next buttons + page number display
[2026-06-14] Result card: title, URL breadcrumb, snippet with highlights
[2026-06-15] Snippet highlights: <mark> tags with yellow background
[2026-06-15] Empty state: magnifier icon + 'No results found' message
[2026-06-16] Loading state: skeleton cards with CSS animation
[2026-06-16] Error state: red alert banner with retry button
[2026-06-17] Dashboard shows: doc count, query count, index size
[2026-06-17] Dashboard polls /api/v1/index/stats every 30s
[2026-06-18] app.js: fetch() wraps API calls with auth header injection
[2026-06-18] API_KEY stored in localStorage (dev only)
[2026-06-19] Theme toggle persists via localStorage 'theme' key
[2026-06-19] CSS prefers-color-scheme: used as initial theme
[2026-06-20] Responsive breakpoints: 768px (tablet), 480px (mobile)
[2026-06-20] Mobile: full-width search bar, single-column results
[2026-06-21] Font: Inter from Google Fonts, weights 400/500/600/700
[2026-06-21] Line height: 1.6 for body text readability
[2026-06-22] Search result: max 2 lines snippet, ellipsis overflow
[2026-06-22] URL breadcrumb: max 60 chars, truncated with …
[2026-06-23] Query time shown in ms below result count
[2026-06-23] Result count: '42 results (3.4 ms)' format
[2026-06-24] Keyboard nav: arrow keys cycle autocomplete, Enter submits
