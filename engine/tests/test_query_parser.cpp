#include <catch2/catch_test_macros.hpp>
#include "query_parser.h"

using namespace search;

TEST_CASE("QueryParser simple single term", "[query_parser]") {
    QueryParser parser;
    auto pq = parser.parse("search");
    REQUIRE(pq.type == ParsedQuery::Type::AND);
    REQUIRE(pq.terms.size() == 1);
    REQUIRE(pq.terms[0] == "search");
    REQUIRE_FALSE(pq.is_phrase);
}

TEST_CASE("QueryParser multi-term AND default", "[query_parser]") {
    QueryParser parser(true);
    auto pq = parser.parse("distributed search engine");
    REQUIRE(pq.type == ParsedQuery::Type::AND);
    REQUIRE(pq.terms.size() == 3);
    REQUIRE(pq.terms[0] == "distributed");
    REQUIRE(pq.terms[1] == "search");
    REQUIRE(pq.terms[2] == "engine");
}

TEST_CASE("QueryParser OR operator detection", "[query_parser]") {
    QueryParser parser;
    auto pq = parser.parse("cpp or python");
    REQUIRE(pq.type == ParsedQuery::Type::OR);
    REQUIRE(pq.terms.size() == 2);
    REQUIRE(pq.terms[0] == "cpp");
    REQUIRE(pq.terms[1] == "python");
}

TEST_CASE("QueryParser quoted phrase parsing", "[query_parser]") {
    QueryParser parser;
    auto pq = parser.parse("\"machine learning\"");
    REQUIRE(pq.is_phrase);
    REQUIRE(pq.type == ParsedQuery::Type::PHRASE);
    REQUIRE(pq.phrase == "machine learning");
    REQUIRE(pq.terms.size() == 2);
    REQUIRE(pq.terms[0] == "machine");
    REQUIRE(pq.terms[1] == "learning");
}
// Test: unicode character preservation
