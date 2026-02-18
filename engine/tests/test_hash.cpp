#include <catch2/catch_test_macros.hpp>
#include "hash.h"

using namespace search;

TEST_CASE("FNV-1a 64-bit hash consistency", "[hash]") {
    auto h1 = HashUtil::fnv1a_64("search engine");
    auto h2 = HashUtil::fnv1a_64("search engine");
    auto h3 = HashUtil::fnv1a_64("different text");

    REQUIRE(h1 == h2);
    REQUIRE(h1 != h3);
    REQUIRE(h1 != 0);
}

TEST_CASE("SimHash near-duplicate distance", "[hash]") {
    std::string doc1 = "the quick brown fox jumps over the lazy dog";
    std::string doc2 = "the quick brown fox jumps over the lazy dog barking";
    std::string doc3 = "quantum mechanics quantum physics universe galaxies";

    uint64_t sim1 = HashUtil::simhash_64(doc1);
    uint64_t sim2 = HashUtil::simhash_64(doc2);
    uint64_t sim3 = HashUtil::simhash_64(doc3);

    int dist_close = HashUtil::hamming_distance(sim1, sim2);
    int dist_far = HashUtil::hamming_distance(sim1, sim3);

    REQUIRE(dist_close < dist_far);
    REQUIRE(dist_close <= 10);
}
