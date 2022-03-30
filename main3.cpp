#include <iostream>
#include <random>
#include "fun.hpp"

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>


namespace py = pybind11;
using namespace py::literals;

int main() {

    py::scoped_interpreter guard{};


    // Create a generator with seed 10 and print it to .txt file
    std::mt19937 gen(10);

    // Create a generator in python
    auto numpy_random = py::module_::import("numpy.random");
    auto py_engine = numpy_random.attr("MT19937")();
    auto py_gen = numpy_random.attr("Generator")(py_engine);
    auto fun = py::module_::import("fun");

    std::cout << gen << std::endl;
    std::cout << "################################" << std::endl;
    synchronize_cpp_to_py_state(gen,py_gen);
    py::print(py_gen.attr("__getstate__")());
    std::cout << "################################" << std::endl;
    std::cout << "The sample is: " << fun.attr("sample_normal")(0,1,py_gen).cast<double>() << std::endl;
    py::print(py_gen.attr("__getstate__")());
    std::cout << "################################" << std::endl;
    synchronize_py_to_cpp_state(gen,py_gen);
    std::cout << gen << std::endl;
    std::cout << "################################" << std::endl;

    return 0;
}

