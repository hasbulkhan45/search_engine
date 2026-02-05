#include <catch2/catch_test_macros.hpp>
#include "tokenizer.h"

using namespace search;

TEST_CASE("Tokenizer basic split", "[tokenizer]") {
    Tokenizer::Config cfg;
    cfg.remove_stopwords = false;
    cfg.apply_stemming   = false;
    Tokenizer tok(cfg);

    auto tokens = tok.tokenize("Hello World foo");
    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == "hello");
    REQUIRE(tokens[1] == "world");
}

TEST_CASE("Tokenizer removes stopwords", "[tokenizer]") {
    Tokenizer tok;
    auto tokens = tok.tokenize("the quick brown fox");
    // "the" is a stopword
    for (auto& t : tokens) {
        REQUIRE(t != "the");
    }
}

TEST_CASE("Tokenizer min length filter", "[tokenizer]") {
    Tokenizer::Config cfg;
    cfg.min_len = 4;
    cfg.remove_stopwords = false;
    cfg.apply_stemming   = false;
    Tokenizer tok(cfg);
    auto tokens = tok.tokenize("a ab abc abcd");
    REQUIRE(tokens.size() == 1);
    REQUIRE(tokens[0] == "abcd");
}

TEST_CASE("Tokenizer custom filter", "[tokenizer]") {
    Tokenizer::Config cfg;
    cfg.remove_stopwords = false;
    cfg.apply_stemming   = false;
    Tokenizer tok(cfg);
    // Drop tokens containing digits
    tok.add_filter([](const std::string& t){
        for (char c : t) if (std::isdigit(c)) return false;
        return true;
    });
    auto tokens = tok.tokenize("hello world 42 foo3 bar");
    for (auto& t : tokens) {
        bool has_digit = false;
        for (char c : t) if (std::isdigit(c)) has_digit = true;
        REQUIRE(!has_digit);
    }
}

TEST_CASE("Tokenizer stemming", "[tokenizer]") {
    Tokenizer tok;
    auto tokens = tok.tokenize("running runs runner");
    // All should stem to "run"
    for (auto& t : tokens) {
        REQUIRE(t == "run");
    }
}
