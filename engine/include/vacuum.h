#pragma once
#include "index.h"
#include <unordered_set>

namespace search {

class IndexVacuum {
public:
    static size_t purge_deleted(Index& index, const std::unordered_set<uint32_t>& tombstoned_ids);
};

} // namespace search
// Vacuum: bloom filter for tombstone ids
