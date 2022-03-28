#include <iostream>
#include <random>
#include <fstream>
#include <cmath>

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>


namespace py = pybind11;
using namespace py::literals;

int main() {

    py::scoped_interpreter guard{};


    // Create a generator with seed 10 and print it to .txt file
    std::mt19937 gen(10);
    std::ofstream state("state_cpp.txt");
    state << gen;
    state.close();

    // Import the state from the .txt file to check that it works
    std::mt19937 gen_new(0);
    std::ifstream file_new("state_cpp.txt");
    file_new >> gen_new;
    file_new.close();

    // Create a generator in python
    auto numpy_random = py::module_::import("numpy.random");
    auto py_engine = numpy_random.attr("MT19937")();
    auto py_gen = numpy_random.attr("Generator")(py_engine);

    // Set the python state and write it in a separate .txt file
    auto fun = py::module_::import("fun");
    fun.attr("set_state_from_txt")(py_gen, "state_cpp.txt");
    fun.attr("write_state_to_txt")(py_gen, "state_py.txt");

    // Create a generator and set its state equal to the python one
    std::mt19937 gen_compare(0);
    std::ifstream file_compare("state_py.txt");
    file_compare >> gen_compare;
    file_compare.close();

    // Compare the states
    if (gen_compare == gen_new) {
        std::cout << "The states in cpp and py are the SAME" << std::endl;
    }
    else {
        std::cout << "The states in cpp and py are DIFFERENT" << std::endl;
    }

    // Generate and print a 32-bit unsigned int in python and cpp
    auto n = std::pow(2,32);
    std::cout << "Sample in python: " << py_gen.attr("integers")(0,n,1,"uint32").cast<unsigned int>() << std::endl;
    std::cout << "Sample in cpp: " << gen_new() << std::endl;

    std::cout << "Sample gaussian in python: " << py_gen.attr("normal")(0,1).cast<double>() << std::endl;
    std::normal_distribution<double> distribution(0,1.0);
    std::cout << "Sample gaussian in cpp: " << distribution(gen_new) << std::endl;

    // Check if the states are still the same

    // Write cpp state
    std::ofstream state_after("state_cpp.txt");
    state_after << gen_new;
    state_after.close();

    // Write python state
    fun.attr("write_state_to_txt")(py_gen, "state_py.txt");
    std::ifstream file_compare_after("state_py.txt");
    file_compare_after >> gen_compare;
    file_compare_after.close();

    if (gen_compare == gen_new) {
        std::cout << "The states after sampling in cpp and py are the SAME" << std::endl;
    }
    else {
        std::cout << "The states after sampling in cpp and py are DIFFERENT" << std::endl;
    }

    return 0;
}