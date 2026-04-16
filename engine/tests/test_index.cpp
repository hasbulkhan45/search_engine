#include <catch2/catch_test_macros.hpp>
#include "index.h"
#include "tokenizer.h"

using namespace search;

static Index make_index() {
    Index idx;
    Tokenizer tok;
    {
        Document d(1, "https://example.com", "Hello World", "The quick brown fox");
        idx.add_document(d, tok.tokenize(d.full_text()));
    }
    {
        Document d(2, "https://foo.com", "Foo Bar", "quick lazy dog");
        idx.add_document(d, tok.tokenize(d.full_text()));
    }
    return idx;
}

TEST_CASE("Index doc count", "[index]") {
    auto idx = make_index();
    REQUIRE(idx.doc_count() == 2);
}

TEST_CASE("Index lookup existing term", "[index]") {
    auto idx = make_index();
    auto* pl = idx.lookup("quick");
    REQUIRE(pl != nullptr);
    REQUIRE(pl->size() >= 1);
}

TEST_CASE("Index lookup missing term", "[index]") {
    auto idx = make_index();
    REQUIRE(idx.lookup("zzzzz") == nullptr);
}

TEST_CASE("Index intersect two terms", "[index]") {
    auto idx = make_index();
    // "quick" appears in both docs
    Tokenizer tok;
    auto res = idx.intersect(tok.tokenize("quick"));
    REQUIRE(!res.empty());
}

TEST_CASE("Index remove document", "[index]") {
    auto idx = make_index();
    idx.remove_document(1);
    REQUIRE(idx.doc_count() == 1);
    REQUIRE(idx.get_doc(1) == nullptr);
}

TEST_CASE("Index avg_doc_len > 0", "[index]") {
    auto idx = make_index();
    REQUIRE(idx.avg_doc_len() > 0.0);
}

TEST_CASE("Index positional phrase search", "[index]") {
    auto idx = make_index();
    Tokenizer tok;

    auto matches = idx.phrase_search(tok.tokenize("quick brown"));
    REQUIRE(matches.size() == 1);
    REQUIRE(matches[0].doc_id == 1);

    // Reversed order should not match
    auto no_matches = idx.phrase_search(tok.tokenize("brown quick"));
    REQUIRE(no_matches.empty());
}
// Test: remove document invalidation
// Test: vacuum with 0 tombstones
