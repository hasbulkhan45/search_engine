#include <catch2/catch_test_macros.hpp>
#include "stop_words.h"

using namespace search;

TEST_CASE("StopWords lookup", "[stopwords]") {
    REQUIRE(StopWords::is_stop_word("the"));
    REQUIRE(StopWords::is_stop_word("and"));
    REQUIRE(StopWords::is_stop_word("is"));
    REQUIRE_FALSE(StopWords::is_stop_word("algorithm"));
    REQUIRE_FALSE(StopWords::is_stop_word("search"));
}
