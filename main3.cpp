#include <iostream>
#include <fstream>
#include <random>
#include "fun.hpp"

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>


namespace py = pybind11;
using namespace py::literals;

int main() {

    py::scoped_interpreter guard{};


    // Create a generator with seed 10 and print it to .txt file
    std::mt19937 gen(156);

    // Create a generator in python
    auto numpy_random = py::module_::import("numpy.random");
    auto py_engine = numpy_random.attr("MT19937")();
    auto py_gen = numpy_random.attr("Generator")(py_engine);
    auto fun = py::module_::import("fun");

    // Write the gen state to txt
    std::ofstream state("1_gen_state_before.txt");
    state << gen;
    state.close();

    synchronize_cpp_to_py_state(gen, py_gen);

    fun.attr("write_state_to_txt")(py_gen, "2_py_gen_from_cpp_state_before.txt");

    std::cout << "The sample is: " << fun.attr("sample_normal")(0, 1, py_gen).cast<double>() << std::endl;

    fun.attr("write_state_to_txt")(py_gen, "5_py_gen_from_cpp_state_after.txt");

    synchronize_py_to_cpp_state(gen, py_gen);

    std::ofstream state_("6_gen_state_after.txt");
    state_ << gen;
    state_.close();

    std::mt19937 gen1;
    std::mt19937 gen2;
    std::mt19937 gen3;
    std::mt19937 gen4;
    std::mt19937 gen5;
    std::mt19937 gen6;

    std::ifstream state1("1_gen_state_before.txt");
    state1 >> gen1;
    state1.close();

    std::ifstream state2("2_py_gen_from_cpp_state_before.txt");
    state2 >> gen2;
    state2.close();

    std::ifstream state3("3_py_gen_from_py_state_before.txt");
    state3 >> gen3;
    state3.close();

    std::ifstream state4("4_py_gen_from_py_state_after.txt");
    state4 >> gen4;
    state4.close();

    std::ifstream state5("5_py_gen_from_cpp_state_after.txt");
    state5 >> gen5;
    state5.close();

    std::ifstream state6("6_gen_state_after.txt");
    state6 >> gen6;
    state6.close();

    if ((gen1 == gen2) && (gen1 == gen3)) {
        std::cout << "States before sampling are equal in all the environments" << std::endl;
    }

    if ((gen4 == gen5) && (gen4 == gen6)) {
        std::cout << "States after sampling are equal in all the environments" << std::endl;
    }

    return 0;
}

