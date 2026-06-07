#include "query_planner.h"
#include <algorithm>

namespace search {

QueryPlanner::QueryPlanner(const Index& index) : index_(index) {}

std::vector<std::string> QueryPlanner::optimize_term_order(const std::vector<std::string>& terms) const {
    if (terms.size() <= 1) return terms;

    std::vector<std::pair<size_t, std::string>> term_costs;
    term_costs.reserve(terms.size());

    for (const auto& t : terms) {
        auto* pl = index_.lookup(t);
        size_t cost = pl ? pl->size() : 0;
        term_costs.emplace_back(cost, t);
    }

    // Sort ascending by posting list length (smallest lists first to maximize intersection filter)
    std::sort(term_costs.begin(), term_costs.end(), [](const auto& a, const auto& b){
        return a.first < b.first;
    });

    std::vector<std::string> ordered;
    ordered.reserve(terms.size());
    for (auto& tc : term_costs) {
        ordered.push_back(std::move(tc.second));
    }
    return ordered;
}

} // namespace search
