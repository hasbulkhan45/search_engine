#pragma once
#include "scorer.h"
#include <string>
#include <unordered_map>
#include <list>
#include <mutex>

namespace search {

class QueryCache {
public:
    explicit QueryCache(size_t capacity = 500);

    bool get(const std::string& query, ResultList& out_results);
    void put(const std::string& query, const ResultList& results);
    size_t size() const;
    void clear();

private:
    size_t capacity_;
    std::list<std::pair<std::string, ResultList>> lru_list_;
    std::unordered_map<std::string, std::list<std::pair<std::string, ResultList>>::iterator> cache_map_;
    mutable std::mutex mu_;
};

} // namespace search
// Metrics: hit_count and miss_count
