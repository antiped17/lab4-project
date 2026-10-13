cmake_minimum_required(VERSION 3.10)
project(sort_analysis CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)  # замеры времени имеют смысл только с оптимизацией
endif()

include_directories(include)
add_executable(benchmark src/benchmark.cpp)
add_executable(tests src/tests.cpp)
