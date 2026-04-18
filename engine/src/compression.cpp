#include "compression.h"
#include <stdexcept>

namespace search {

void Compression::encode_varint(uint32_t value, std::vector<uint8_t>& out) {
    while (value >= 0x80) {
        out.push_back(static_cast<uint8_t>((value & 0x7F) | 0x80));
        value >>= 7;
    }
    out.push_back(static_cast<uint8_t>(value & 0x7F));
}

uint32_t Compression::decode_varint(const std::vector<uint8_t>& in, size_t& offset) {
    uint32_t result = 0;
    uint32_t shift = 0;

    while (offset < in.size()) {
        uint8_t byte = in[offset++];
        result |= static_cast<uint32_t>(byte & 0x7F) << shift;
        if ((byte & 0x80) == 0) {
            return result;
        }
        shift += 7;
        if (shift >= 35) {
            throw std::runtime_error("Malformed varint in stream");
        }
    }
    throw std::runtime_error("Unexpected end of varint stream");
}

std::vector<uint32_t> Compression::delta_encode(const std::vector<uint32_t>& sorted_numbers) {
    if (sorted_numbers.empty()) return {};
    std::vector<uint32_t> deltas;
    deltas.reserve(sorted_numbers.size());

    deltas.push_back(sorted_numbers[0]);
    for (size_t i = 1; i < sorted_numbers.size(); ++i) {
        deltas.push_back(sorted_numbers[i] - sorted_numbers[i - 1]);
    }
    return deltas;
}

std::vector<uint32_t> Compression::delta_decode(const std::vector<uint32_t>& deltas) {
    if (deltas.empty()) return {};
    std::vector<uint32_t> numbers;
    numbers.reserve(deltas.size());

    uint32_t running = deltas[0];
    numbers.push_back(running);
    for (size_t i = 1; i < deltas.size(); ++i) {
        running += deltas[i];
        numbers.push_back(running);
    }
    return numbers;
}

std::vector<uint8_t> Compression::compress_sorted(const std::vector<uint32_t>& sorted_numbers) {
    if (sorted_numbers.empty()) return {};
    auto deltas = delta_encode(sorted_numbers);
    std::vector<uint8_t> out;
    // Store count first
    encode_varint(static_cast<uint32_t>(deltas.size()), out);
    for (uint32_t d : deltas) {
        encode_varint(d, out);
    }
    return out;
}

std::vector<uint32_t> Compression::decompress_sorted(const std::vector<uint8_t>& bytes) {
    if (bytes.empty()) return {};
    size_t offset = 0;
    uint32_t count = decode_varint(bytes, offset);

    std::vector<uint32_t> deltas;
    deltas.reserve(count);
    for (uint32_t i = 0; i < count; ++i) {
        deltas.push_back(decode_varint(bytes, offset));
    }
    return delta_decode(deltas);
}

} // namespace search
// Perf: unlikely() macro on varint continuation
