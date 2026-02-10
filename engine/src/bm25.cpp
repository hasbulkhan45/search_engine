#include "bm25.h"
#include <cmath>
#include <algorithm>
#include <unordered_map>

namespace search {

BM25Scorer::BM25Scorer(const Index& index)
    : BM25Scorer(index, Params{}) {}

BM25Scorer::BM25Scorer(const Index& index, const Params& params)
    : index_(index), p_(params) {}

double BM25Scorer::idf(const std::string& term) const {
    size_t N  = index_.doc_count();
    auto*  pl = index_.lookup(term);
    size_t df = pl ? pl->size() : 0;
    if (df == 0) return 0.0;
    // Robertson-Sparck Jones IDF with smoothing
    return std::log((N - df + 0.5) / (df + 0.5) + 1.0);
}

double BM25Scorer::score_term(const std::string& term,
                               uint32_t freq, size_t doc_len) const {
    double avgdl = index_.avg_doc_len();
    if (avgdl <= 0.0) return 0.0;
    double tf  = static_cast<double>(freq);
    double len_norm = 1.0 - p_.b + p_.b * (static_cast<double>(doc_len) / avgdl);
    return idf(term) * (tf * (p_.k1 + 1.0)) / (tf + p_.k1 * len_norm);
}

ResultList BM25Scorer::score(const std::vector<std::string>& terms) const {
    std::unordered_map<uint32_t, double> accum;

    for (auto& term : terms) {
        auto* pl = index_.lookup(term);
        if (!pl) continue;

        for (auto& posting : *pl) {
            auto* doc = index_.get_doc(posting.doc_id);
            if (!doc) continue;
            accum[posting.doc_id] +=
                score_term(term, posting.freq, doc->content_len);
        }
    }

    ResultList results;
    results.reserve(accum.size());
    for (auto& [id, sc] : accum)
        results.push_back({id, sc});

    std::sort(results.begin(), results.end(),
              [](const SearchResult& a, const SearchResult& b){
                  return a.score > b.score;
              });
    return results;
}

} // namespace search
