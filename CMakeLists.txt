cmake_minimum_required(VERSION 3.4...3.18)
project(rng_experiments)

find_package(pybind11 REQUIRED)

add_executable(rng_experiments main.cpp)

add_executable(rng_experiments2 main2.cpp)

add_executable(rng_experiments3 main3.cpp)

add_executable(rng_experiments4 main4.cpp)

target_link_libraries(rng_experiments PRIVATE pybind11::embed pybind11::pybind11)

target_link_libraries(rng_experiments2 PRIVATE pybind11::embed pybind11::pybind11)

target_link_libraries(rng_experiments3 PRIVATE pybind11::embed pybind11::pybind11)

target_link_libraries(rng_experiments4 PRIVATE pybind11::embed pybind11::pybind11)