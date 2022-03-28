cmake_minimum_required(VERSION 3.4...3.18)
project(rng_experiments)

find_package(pybind11 REQUIRED)

add_executable(rng_experiments main.cpp)

target_link_libraries(rng_experiments PRIVATE pybind11::embed pybind11::pybind11)