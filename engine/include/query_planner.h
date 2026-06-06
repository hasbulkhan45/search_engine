#pragma once
#include "query_parser.h"
#include "index.h"
#include <vector>
#include <string>

namespace search {

class QueryPlanner {
public:
    explicit QueryPlanner(const Index& index);
    std::vector<std::string> optimize_term_order(const std::vector<std::string>& terms) const;

private:
    const Index& index_;
};

} // namespace search
