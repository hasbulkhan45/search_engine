cmake_minimum_required(VERSION 3.16)
project(SearchEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0 -Wall -Wextra -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

# Include paths
include_directories(engine/include)

add_library(search_engine STATIC
    engine/src/document.cpp
    engine/src/tokenizer.cpp
    engine/src/stemmer.cpp
    engine/src/index.cpp
    engine/src/scorer.cpp
    engine/src/bm25.cpp
    engine/src/serializer.cpp
    engine/src/query_parser.cpp
    engine/src/snippet.cpp
)

target_include_directories(search_engine PUBLIC engine/include)

# Tests
enable_testing()
add_subdirectory(engine/tests)
