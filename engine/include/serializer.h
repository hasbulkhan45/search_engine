#pragma once
#include "index.h"
#include <string>
#include <vector>
#include <fstream>

namespace search {

/**
 * Binary serializer for the inverted index.
 * Format: magic header | doc count | vocab size | postings | documents
 */
class Serializer {
public:
    static constexpr uint32_t MAGIC   = 0x53455243; // 'SERC'
    static constexpr uint32_t VERSION = 1;

    void save(const Index& idx, const std::string& path) const;
    void load(Index& idx, const std::string& path) const;

private:
    static void write_str(std::ofstream& f, const std::string& s);
    static std::string read_str(std::ifstream& f);
    static void write_u32(std::ofstream& f, uint32_t v);
    static uint32_t read_u32(std::ifstream& f);
    static void write_f64(std::ofstream& f, double v);
    static double read_f64(std::ifstream& f);
};

} // namespace search
// Alignment: 16-byte boundary alignment
