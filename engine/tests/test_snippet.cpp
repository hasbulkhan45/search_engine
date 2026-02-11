#include <catch2/catch_test_macros.hpp>
#include "snippet.h"

using namespace search;

TEST_CASE("SnippetGenerator empty text returns empty", "[snippet]") {
    SnippetGenerator gen(100);
    auto snip = gen.generate("", {"query"});
    REQUIRE(snip.text.empty());
    REQUIRE_FALSE(snip.has_match);
}

TEST_CASE("SnippetGenerator extracts window around matching query", "[snippet]") {
    SnippetGenerator gen(60);
    std::string doc = "Introduction to modern operating systems. Distributed systems allow multiple nodes to coordinate tasks across networks seamlessly.";
    auto snip = gen.generate(doc, {"distributed"});

    REQUIRE(snip.has_match);
    REQUIRE(snip.text.find("Distributed") != std::string::npos);
    REQUIRE(snip.text.size() <= 80);
}

TEST_CASE("SnippetGenerator fallback when no term matches", "[snippet]") {
    SnippetGenerator gen(30);
    std::string doc = "Short text with no match.";
    auto snip = gen.generate(doc, {"nonexistent"});

    REQUIRE_FALSE(snip.has_match);
    REQUIRE(snip.text == "Short text with no match.");
}
