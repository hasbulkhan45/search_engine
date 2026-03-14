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
