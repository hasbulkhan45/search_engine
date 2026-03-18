#include <catch2/catch_test_macros.hpp>
#include "disk_index.h"
#include "tokenizer.h"
#include <filesystem>

using namespace search;

TEST_CASE("DiskIndexWriter and Reader segment roundtrip", "[disk_index]") {
    std::string test_dir = "/tmp/test_disk_idx";
    std::filesystem::remove_all(test_dir);

    Index original;
    Tokenizer tok;
    Document d1(1, "https://example.com/disk", "Disk Storage", "Persisting inverted indexes to non-volatile storage");
    original.add_document(d1, tok.tokenize(d1.full_text()));

    DiskIndexWriter writer(test_dir);
    writer.write(original);

    Index loaded;
    DiskIndexReader reader(test_dir);
    REQUIRE(reader.load(loaded));
    REQUIRE(loaded.doc_count() == 1);
    REQUIRE(loaded.get_doc(1) != nullptr);

    std::filesystem::remove_all(test_dir);
}
