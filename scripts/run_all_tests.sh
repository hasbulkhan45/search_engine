#!/bin/bash
set -e
echo "Running C++ Catch2 test suites..."
cd build && ctest --output-on-failure
cd ..
echo "Running Python pytest suites..."
PYTHONPATH=. pytest tests/ -v
echo "All tests passed successfully!"
