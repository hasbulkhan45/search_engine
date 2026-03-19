#pragma once
#include <cstdint>
#include <string>

namespace search {

struct FieldWeights {
    double title_weight       = 3.0;
    double url_weight         = 2.0;
    double meta_desc_weight   = 1.5;
    double body_weight        = 1.0;
};

class FieldScorer {
public:
    explicit FieldScorer(const FieldWeights& weights = {});
    double compute_field_boost(bool in_title, bool in_url, bool in_meta, bool in_body) const;

private:
    FieldWeights weights_;
};

} // namespace search
