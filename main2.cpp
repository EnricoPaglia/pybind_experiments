#include <iostream>
#include <random>
#include "fun.hpp"

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>


namespace py = pybind11;
using namespace py::literals;


int main() {

    py::scoped_interpreter guard{};

    // Create 2 generators with different seeds
    std::mt19937 gen(10);
    std::mt19937 gen2(0);

    // Create a generator in python

    auto numpy_random = py::module_::import("numpy.random");
    auto py_engine = numpy_random.attr("MT19937")();
    auto py_gen = numpy_random.attr("Generator")(py_engine);

    // Synchronize py_gen with gen and then gen2 with py_gen
    synchronize_cpp_to_py_state(gen, py_gen);
    synchronize_py_to_cpp_state(gen2, py_gen);

    // Check if the states are equal
    if (gen == gen2) {
        std::cout << "The states are equal" << std::endl;
    } else {
        std::cout << "The states are different" << std::endl;
    }

    return 0;
}


