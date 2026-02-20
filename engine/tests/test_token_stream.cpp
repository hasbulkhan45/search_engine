#include <catch2/catch_test_macros.hpp>
#include "token_stream.h"

using namespace search;

TEST_CASE("TokenStream extraction", "[token_stream]") {
    std::string text = "   modern  high-performance   c++17 search  ";
    TokenStream ts(text);

    std::vector<std::string> tokens;
    while (ts.has_next()) {
        tokens.emplace_back(ts.next());
    }

    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0] == "modern");
    REQUIRE(tokens[1] == "high-performance");
    REQUIRE(tokens[2] == "c++17");
    REQUIRE(tokens[3] == "search");
}
