#pragma once
#include "index.h"
#include <vector>
#include <memory>
#include <mutex>

namespace search {

class SegmentManager {
public:
    SegmentManager();
    void add_segment(std::unique_ptr<Index> segment);
    size_t segment_count() const;
    void merge_segments(Index& merged_output);

private:
    std::vector<std::unique_ptr<Index>> segments_;
    mutable std::mutex mu_;
};

} // namespace search
// Config: max_unmerged_segments = 8
// Compaction: size-tiered merge policy
// Callback: on_merge_progress(float percent)
// Thread: background compaction worker
