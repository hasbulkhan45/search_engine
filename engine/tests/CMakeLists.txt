cmake_minimum_required(VERSION 3.16)

# Fetch Catch2 for unit testing
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(test_tokenizer test_tokenizer.cpp)
target_link_libraries(test_tokenizer PRIVATE search_engine Catch2::Catch2WithMain)
add_test(NAME TokenizerTests COMMAND test_tokenizer)

add_executable(test_index test_index.cpp)
target_link_libraries(test_index PRIVATE search_engine Catch2::Catch2WithMain)
add_test(NAME IndexTests COMMAND test_index)

add_executable(test_query_parser test_query_parser.cpp)
target_link_libraries(test_query_parser PRIVATE search_engine Catch2::Catch2WithMain)
add_test(NAME QueryParserTests COMMAND test_query_parser)

add_executable(test_bm25 test_bm25.cpp)
target_link_libraries(test_bm25 PRIVATE search_engine Catch2::Catch2WithMain)
add_test(NAME BM25Tests COMMAND test_bm25)

add_executable(test_scorer test_scorer.cpp)
target_link_libraries(test_scorer PRIVATE search_engine Catch2::Catch2WithMain)
add_test(NAME ScorerTests COMMAND test_scorer)

add_executable(test_serializer test_serializer.cpp)
target_link_libraries(test_serializer PRIVATE search_engine Catch2::Catch2WithMain)
add_test(NAME SerializerTests COMMAND test_serializer)

add_executable(test_snippet test_snippet.cpp)
target_link_libraries(test_snippet PRIVATE search_engine Catch2::Catch2WithMain)
add_test(NAME SnippetTests COMMAND test_snippet)

add_executable(test_compression test_compression.cpp)
target_link_libraries(test_compression PRIVATE search_engine Catch2::Catch2WithMain)
add_test(NAME CompressionTests COMMAND test_compression)
# Benchmarks registered
# CI test reporter options
