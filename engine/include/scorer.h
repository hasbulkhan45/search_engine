#pragma once
#include "index.h"
#include <vector>
#include <utility>

namespace search {

struct SearchResult {
    uint32_t doc_id;
    double   score;
    bool operator>(const SearchResult& o) const { return score > o.score; }
};

using ResultList = std::vector<SearchResult>;

/**
 * TF-IDF scorer.
 * score(t,d) = tf(t,d) * idf(t)
 * idf(t) = log( (N - df(t) + 0.5) / (df(t) + 0.5) ) + 1
 */
class TFIDFScorer {
public:
    explicit TFIDFScorer(const Index& index) : index_(index) {}

    ResultList score(const std::vector<std::string>& terms) const;
    double     idf(const std::string& term) const;
    double     tf(uint32_t freq, size_t doc_len) const;

private:
    const Index& index_;
};

} // namespace search
