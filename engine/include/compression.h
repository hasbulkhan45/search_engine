#pragma once
#include <vector>
#include <cstdint>
#include <cstddef>

namespace search {

/**
 * Variable-byte (Varint) and Delta compression for postings lists.
 * Standard information retrieval technique for compact index storage.
 */
class Compression {
public:
    /// Encode a single uint32_t into variable-length bytes (LEB128/Varint)
    static void encode_varint(uint32_t value, std::vector<uint8_t>& out);

    /// Decode a variable-length byte stream starting at offset into a uint32_t
    static uint32_t decode_varint(const std::vector<uint8_t>& in, size_t& offset);

    /// Delta-encode a sorted list of doc IDs: [10, 15, 22] -> [10, 5, 7]
    static std::vector<uint32_t> delta_encode(const std::vector<uint32_t>& sorted_numbers);

    /// Delta-decode gaps back to original numbers: [10, 5, 7] -> [10, 15, 22]
    static std::vector<uint32_t> delta_decode(const std::vector<uint32_t>& deltas);

    /// Combined pipeline: sorted uint32_t list -> delta encoded -> varint compressed bytes
    static std::vector<uint8_t> compress_sorted(const std::vector<uint32_t>& sorted_numbers);

    /// Inverse pipeline: varint compressed bytes -> delta decoded -> original sorted uint32_t list
    static std::vector<uint32_t> decompress_sorted(const std::vector<uint8_t>& bytes);
};

} // namespace search
