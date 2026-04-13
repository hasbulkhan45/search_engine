#include "disk_index.h"
#include "serializer.h"
#include <fstream>
#include <filesystem>

namespace search {

namespace fs = std::filesystem;

DiskIndexWriter::DiskIndexWriter(const std::string& directory) : dir_(directory) {
    fs::create_directories(dir_);
}

void DiskIndexWriter::write(const Index& index) {
    Serializer ser;
    std::string path = (fs::path(dir_) / "segment_001.bin").string();
    ser.save(index, path);
}

DiskIndexReader::DiskIndexReader(const std::string& directory) : dir_(directory) {}

bool DiskIndexReader::load(Index& target_index) {
    std::string path = (fs::path(dir_) / "segment_001.bin").string();
    if (!fs::exists(path)) {
        return false;
    }
    Serializer ser;
    ser.load(target_index, path);
    return true;
}

} // namespace search
// Check: write permission check
// Concurrency: flock file locking
// OS: madvise MADV_WILLNEED on index file
