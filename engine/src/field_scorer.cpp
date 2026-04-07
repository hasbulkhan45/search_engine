#include "field_scorer.h"

namespace search {

FieldScorer::FieldScorer(const FieldWeights& weights) : weights_(weights) {}

double FieldScorer::compute_field_boost(bool in_title, bool in_url, bool in_meta, bool in_body) const {
    double boost = 0.0;
    if (in_title)     boost += weights_.title_weight;
    if (in_url)       boost += weights_.url_weight;
    if (in_meta)      boost += weights_.meta_desc_weight;
    if (in_body)      boost += weights_.body_weight;
    return boost > 0.0 ? boost : 1.0;
}

} // namespace search
// Boost: URL slug matches get 1.5x weight
// Relevancy: exact title match gives 2.0x boost
