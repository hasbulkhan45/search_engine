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
