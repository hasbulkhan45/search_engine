#include "query_cache.h"

namespace search {

QueryCache::QueryCache(size_t capacity) : capacity_(capacity) {}

bool QueryCache::get(const std::string& query, ResultList& out_results) {
    std::lock_guard<std::mutex> lock(mu_);
    auto it = cache_map_.find(query);
    if (it == cache_map_.end()) return false;

    // Move to front of LRU list
    lru_list_.splice(lru_list_.begin(), lru_list_, it->second);
    out_results = it->second->second;
    return true;
}

void QueryCache::put(const std::string& query, const ResultList& results) {
    std::lock_guard<std::mutex> lock(mu_);
    auto it = cache_map_.find(query);
    if (it != cache_map_.end()) {
        it->second->second = results;
        lru_list_.splice(lru_list_.begin(), lru_list_, it->second);
        return;
    }

    if (cache_map_.size() >= capacity_) {
        auto last = lru_list_.end();
        --last;
        cache_map_.erase(last->first);
        lru_list_.pop_back();
    }

    lru_list_.emplace_front(query, results);
    cache_map_[query] = lru_list_.begin();
}

size_t QueryCache::size() const {
    std::lock_guard<std::mutex> lock(mu_);
    return cache_map_.size();
}

void QueryCache::clear() {
    std::lock_guard<std::mutex> lock(mu_);
    cache_map_.clear();
    lru_list_.clear();
}

} // namespace search
// Normalize: sort tokens before key hash
