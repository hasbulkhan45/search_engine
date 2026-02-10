#pragma once
#include "index.h"
#include "scorer.h"

namespace search {

/**
 * BM25 (Best Match 25) ranker — state-of-the-art probabilistic ranking.
 *
 * score(q,d) = Σ IDF(qi) * [ tf(qi,d) * (k1+1) ] / [ tf(qi,d) + k1*(1-b+b*|d|/avgdl) ]
 *
 * Default params: k1=1.5, b=0.75
 */
class BM25Scorer {
public:
    struct Params {
        double k1 = 1.5;
        double b  = 0.75;
        Params() = default;
        Params(double k, double b_val) : k1(k), b(b_val) {}
    };

    explicit BM25Scorer(const Index& index);
    BM25Scorer(const Index& index, const Params& params);

    ResultList score(const std::vector<std::string>& terms) const;
    double     score_term(const std::string& term,
                          uint32_t freq, size_t doc_len) const;
    double     idf(const std::string& term) const;

    const Params& params() const { return p_; }

private:
    const Index& index_;
    Params       p_;
};

} // namespace search
