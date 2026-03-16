#pragma once
#include "index.h"
#include <string>
#include <vector>

namespace search {

class DiskIndexWriter {
public:
    explicit DiskIndexWriter(const std::string& directory);
    void write(const Index& index);

private:
    std::string dir_;
};

class DiskIndexReader {
public:
    explicit DiskIndexReader(const std::string& directory);
    bool load(Index& target_index);

private:
    std::string dir_;
};

} // namespace search
