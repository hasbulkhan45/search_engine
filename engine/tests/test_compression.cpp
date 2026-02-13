#include <catch2/catch_test_macros.hpp>
#include "compression.h"

using namespace search;

TEST_CASE("Varint encode and decode single value", "[compression]") {
    std::vector<uint8_t> buffer;
    Compression::encode_varint(1, buffer);
    REQUIRE(buffer.size() == 1);
    REQUIRE(buffer[0] == 1);

    size_t offset = 0;
    REQUIRE(Compression::decode_varint(buffer, offset) == 1);
    REQUIRE(offset == 1);

    // Multi-byte value
    buffer.clear();
    Compression::encode_varint(300, buffer);
    REQUIRE(buffer.size() == 2);
    offset = 0;
    REQUIRE(Compression::decode_varint(buffer, offset) == 300);
    REQUIRE(offset == 2);
}

TEST_CASE("Delta encoding and decoding round-trip", "[compression]") {
    std::vector<uint32_t> original = {10, 15, 22, 100, 105, 500};
    auto deltas = Compression::delta_encode(original);
    REQUIRE(deltas == std::vector<uint32_t>{10, 5, 7, 78, 5, 395});

    auto decoded = Compression::delta_decode(deltas);
    REQUIRE(decoded == original);
}

TEST_CASE("Full compress and decompress sorted list", "[compression]") {
    std::vector<uint32_t> doc_ids = {3, 10, 25, 40, 41, 1024, 2048, 50000};
    auto compressed = Compression::compress_sorted(doc_ids);

    // Compressed bytes should be significantly fewer than 8 * 4 = 32 bytes
    REQUIRE(compressed.size() < doc_ids.size() * sizeof(uint32_t));

    auto decompressed = Compression::decompress_sorted(compressed);
    REQUIRE(decompressed == doc_ids);
}
