#include "vacuum.h"

namespace search {

size_t IndexVacuum::purge_deleted(Index& index, const std::unordered_set<uint32_t>& tombstoned_ids) {
    size_t purged = 0;
    for (uint32_t id : tombstoned_ids) {
        index.remove_document(id);
        purged++;
    }
    return purged;
}

} // namespace search
// Memory: stream-based posting compaction
