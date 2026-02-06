#include "scorer.h"
#include <cmath>
#include <algorithm>
#include <unordered_map>

namespace search {

double TFIDFScorer::idf(const std::string& term) const {
    size_t N  = index_.doc_count();
    auto*  pl = index_.lookup(term);
    size_t df = pl ? pl->size() : 0;
    if (df == 0) return 0.0;
    return std::log((static_cast<double>(N) - df + 0.5) / (df + 0.5)) + 1.0;
}

double TFIDFScorer::tf(uint32_t freq, size_t doc_len) const {
    if (doc_len == 0) return 0.0;
    return static_cast<double>(freq) / doc_len;
}

ResultList TFIDFScorer::score(const std::vector<std::string>& terms) const {
    std::unordered_map<uint32_t, double> accum;

    for (auto& term : terms) {
        double idf_val = idf(term);
        if (idf_val <= 0.0) continue;

        auto* pl = index_.lookup(term);
        if (!pl) continue;

        for (auto& posting : *pl) {
            auto* doc = index_.get_doc(posting.doc_id);
            if (!doc) continue;
            double tf_val = tf(posting.freq, doc->content_len + 1);
            accum[posting.doc_id] += tf_val * idf_val;
        }
    }

    ResultList results;
    results.reserve(accum.size());
    for (auto& [id, score] : accum)
        results.push_back({id, score});

    std::sort(results.begin(), results.end(),
              [](const SearchResult& a, const SearchResult& b){
                  return a.score > b.score;
              });
    return results;
}

} // namespace search
