#include "segment_manager.h"

namespace search {

SegmentManager::SegmentManager() = default;

void SegmentManager::add_segment(std::unique_ptr<Index> segment) {
    std::lock_guard<std::mutex> lock(mu_);
    segments_.push_back(std::move(segment));
}

size_t SegmentManager::segment_count() const {
    std::lock_guard<std::mutex> lock(mu_);
    return segments_.size();
}

void SegmentManager::merge_segments(Index& merged_output) {
    std::lock_guard<std::mutex> lock(mu_);
    for (auto& seg : segments_) {
        for (const auto& [id, doc] : seg->documents()) {
            merged_output.add_document(doc, {});
        }
    }
    segments_.clear();
}

} // namespace search
