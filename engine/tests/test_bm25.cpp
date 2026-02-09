#include <catch2/catch_test_macros.hpp>
#include "bm25.h"
#include "index.h"
#include "tokenizer.h"

using namespace search;

TEST_CASE("BM25 scoring basics", "[bm25]") {
    Index idx;
    Tokenizer tok;

    Document d1(1, "https://doc1.com", "Distributed Systems", "Distributed computing systems handle large scale storage and processing");
    Document d2(2, "https://doc2.com", "Operating Systems", "Operating systems manage hardware memory and processes");
    Document d3(3, "https://doc3.com", "Cooking Recipes", "Delicious pasta and pizza recipes for dinner");

    idx.add_document(d1, tok.tokenize(d1.full_text()));
    idx.add_document(d2, tok.tokenize(d2.full_text()));
    idx.add_document(d3, tok.tokenize(d3.full_text()));

    BM25Scorer ranker(idx);
    auto query_tokens = tok.tokenize("distributed systems");
    auto results = ranker.score(query_tokens);

    REQUIRE_FALSE(results.empty());
    // Doc 1 should be the highest ranked document for "distributed systems"
    REQUIRE(results[0].doc_id == 1);
    REQUIRE(results[0].score > 0.0);
}

TEST_CASE("BM25 non-matching query returns empty or zero", "[bm25]") {
    Index idx;
    Tokenizer tok;
    Document d1(1, "https://doc1.com", "Title", "Some random body text");
    idx.add_document(d1, tok.tokenize(d1.full_text()));

    BM25Scorer ranker(idx);
    auto results = ranker.score(tok.tokenize("nonexistenttermxyz"));
    REQUIRE(results.empty());
}
