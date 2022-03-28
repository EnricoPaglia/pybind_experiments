#include <iostream>
#include <random>
#include <fstream>
#include <cmath>

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>


namespace py = pybind11;
using namespace py::literals;

void compare(std::string state_cpp, std::string state_py, std::mt19937 gen_comp, std::mt19937 gen_new_, py::object py_gen, py::module_ fun);


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
    } else {
        std::cout << "The states in cpp and py are DIFFERENT" << std::endl;
    }

    // Generate and print a 32-bit unsigned int in python and cpp
    auto n = std::pow(2, 32);
    std::cout << "Sample in python: " << py_gen.attr("integers")(0, n, 1, "uint32").cast<unsigned int>() << std::endl;
    std::cout << "Sample in cpp: " << gen_new() << std::endl;

    compare("state_cpp.txt", "state_py.txt", gen_compare, gen_new, py_gen, fun);

    // Generate and print samples from normal distribution in python and cpp
    std::normal_distribution<> normal(0, 1);
    std::cout << "Normal sample in python: " << py_gen.attr("normal")(0, 1, 1).cast<double>() << std::endl;
    std::cout << "Normal sample in cpp: " << normal(gen_new) << std::endl;

    compare("state_cpp.txt", "state_py.txt", gen_compare, gen_new, py_gen, fun);

    // Generate and print samples from uniform distribution in python and cpp
    std::uniform_int_distribution<> unif(1, 6);
    std::cout << "Uniform sample in python: " << py_gen.attr("uniform")(1, 6, 1).cast<double>() << std::endl;
    std::cout << "Uniform sample in cpp: " << unif(gen_new) << std::endl;

    compare("state_cpp.txt", "state_py.txt", gen_compare, gen_new, py_gen, fun);

    return 0;
}

void compare(std::string state_cpp, std::string state_py, std::mt19937 gen_comp, std::mt19937 gen_new_, py::object py_gen, py::module_ fun) {
    std::ofstream state_after(state_cpp);
    state_after << gen_new_;
    state_after.close();

    // Write python state
    fun.attr("write_state_to_txt")(py_gen, state_py);
    std::ifstream file_compare_after("state_py.txt");
    file_compare_after >> gen_comp;
    file_compare_after.close();

    if (gen_comp == gen_new_) {
        std::cout << "The states after sampling in cpp and py are the SAME" << std::endl;
    } else {
        std::cout << "The states after sampling in cpp and py are DIFFERENT" << std::endl;
    }
}