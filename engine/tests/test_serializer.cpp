#include <catch2/catch_test_macros.hpp>
#include "serializer.h"
#include "index.h"
#include "tokenizer.h"
#include <cstdio>

using namespace search;

TEST_CASE("Serializer save and load", "[serializer]") {
    const std::string tmp_path = "/tmp/test_index_ser.bin";

    Index original_idx;
    Tokenizer tok;
    Document d1(101, "https://engine.io", "Search Systems", "Inverted index and fast retrieval");
    Document d2(102, "https://engine.io/about", "About Us", "Search engine developers and contributors");

    original_idx.add_document(d1, tok.tokenize(d1.full_text()));
    original_idx.add_document(d2, tok.tokenize(d2.full_text()));

    Serializer ser;
    ser.save(original_idx, tmp_path);

    Index loaded_idx;
    ser.load(loaded_idx, tmp_path);

    REQUIRE(loaded_idx.doc_count() == 2);
    auto* loaded_d1 = loaded_idx.get_doc(101);
    REQUIRE(loaded_d1 != nullptr);
    REQUIRE(loaded_d1->url == "https://engine.io");
    REQUIRE(loaded_d1->title == "Search Systems");

    std::remove(tmp_path.c_str());
}
