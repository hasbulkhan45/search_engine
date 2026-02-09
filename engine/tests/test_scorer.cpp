#include <catch2/catch_test_macros.hpp>
#include "scorer.h"
#include "index.h"
#include "tokenizer.h"

using namespace search;

TEST_CASE("TFIDFScorer IDF calculation", "[scorer]") {
    Index idx;
    Tokenizer tok;
    Document d1(1, "https://example.com/1", "AI Research", "Artificial intelligence neural networks");
    Document d2(2, "https://example.com/2", "Web Search", "Web indexing information retrieval algorithms");
    idx.add_document(d1, tok.tokenize(d1.full_text()));
    idx.add_document(d2, tok.tokenize(d2.full_text()));

    TFIDFScorer scorer(idx);
    double idf_ai = scorer.idf("artifici");
    REQUIRE(idf_ai > 0.0);

    double idf_missing = scorer.idf("quantumphysics");
    REQUIRE(idf_missing == 0.0);
}

TEST_CASE("TFIDFScorer ranking order", "[scorer]") {
    Index idx;
    Tokenizer tok;
    Document d1(1, "https://a.com", "Graph Theory", "Graph algorithms graph traversal graph coloring");
    Document d2(2, "https://b.com", "Tree Data Structures", "Binary trees balanced search trees traversal");
    idx.add_document(d1, tok.tokenize(d1.full_text()));
    idx.add_document(d2, tok.tokenize(d2.full_text()));

    TFIDFScorer scorer(idx);
    auto results = scorer.score(tok.tokenize("graph algorithms"));
    REQUIRE_FALSE(results.empty());
    REQUIRE(results[0].doc_id == 1);
}
