cmake_minimum_required(VERSION 3.17)
project(cache)

set(CMAKE_CXX_STANDARD 11)

add_executable(cache cache2_0.cpp test_generator.cpp cache_header.h)