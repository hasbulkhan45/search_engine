cmake_minimum_required(VERSION 3.16)
project(SearchEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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
    engine/src/compression.cpp
    engine/src/stop_words.cpp
    engine/src/hash.cpp
    engine/src/token_stream.cpp
    engine/src/disk_index.cpp
    engine/src/field_scorer.cpp
    engine/src/segment_manager.cpp
    engine/src/vacuum.cpp
    engine/src/query_planner.cpp
    engine/src/query_cache.cpp
)

target_include_directories(search_engine PUBLIC engine/include)

# Python bindings
find_package(pybind11 CONFIG QUIET)
if(NOT pybind11_FOUND)
  find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
  execute_process(
    COMMAND "${Python3_EXECUTABLE}" -m pybind11 --cmakedir
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE pybind11_DIR
  )
  find_package(pybind11 CONFIG REQUIRED)
endif()

pybind11_add_module(search_engine_native bindings/search_engine_py.cpp)
target_link_libraries(search_engine_native PRIVATE search_engine)
set_target_properties(search_engine_native PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# Tests
enable_testing()
add_subdirectory(engine/tests)
# Phase 2 finalized
# Target: install search_engine_native

# Final C++ compiler flags optimization
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG -flto")
