#include <catch2/catch_test_macros.hpp>
#include "field_scorer.h"

using namespace search;

TEST_CASE("FieldScorer boosting logic", "[field_scorer]") {
    FieldWeights weights;
    weights.title_weight = 4.0;
    weights.body_weight  = 1.0;
    FieldScorer scorer(weights);

    double title_match = scorer.compute_field_boost(true, false, false, false);
    double body_match  = scorer.compute_field_boost(false, false, false, true);

    REQUIRE(title_match > body_match);
    REQUIRE(title_match == 4.0);
    REQUIRE(body_match == 1.0);
}
// Test: multi-field weight combinations
