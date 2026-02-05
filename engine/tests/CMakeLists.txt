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
