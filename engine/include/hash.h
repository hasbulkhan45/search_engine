#pragma once
#include <string>
#include <cstdint>
#include <string_view>

namespace search {

class HashUtil {
public:
    /// 64-bit FNV-1a hash
    static uint64_t fnv1a_64(std::string_view text);

    /// SimHash 64-bit fingerprint for near-duplicate text detection
    static uint64_t simhash_64(const std::string& text);

    /// Hamming distance between two 64-bit fingerprints
    static int hamming_distance(uint64_t a, uint64_t b);
};

} // namespace search
