#include <iostream>
#include <random>
#include <fstream>
#include <cmath>
#include <sstream>
#include <string>

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>


namespace py = pybind11;
using namespace py::literals;

void synchronize_cpp_to_py_state(const std::mt19937 & cpp_gen, py::object & py_gen, py::module_ & numpy);
void synchronize_py_to_cpp_state(std::mt19937 & cpp_gen,const py::object & py_gen);

int main(){

    py::scoped_interpreter guard{};

    // Create a generator with seed 10 and save it in a std::stringstream object
    std::mt19937 gen(10);
    std::mt19937 gen2(0);

    // Create a generator in python
    auto numpy = py::module_::import("numpy");
    auto numpy_random = py::module_::import("numpy.random");
    auto py_engine = numpy_random.attr("MT19937")();
    auto py_gen = numpy_random.attr("Generator")(py_engine);

    synchronize_cpp_to_py_state(gen,py_gen,numpy);
    synchronize_py_to_cpp_state(gen2,py_gen);

    if(gen==gen2){
        std::cout << "The states are the same" << std::endl;
    } else{
        std::cout << "The states are different" << std::endl;
    }

    return 0;
}

void synchronize_cpp_to_py_state(const std::mt19937 & cpp_gen, py::object & py_gen, py::module_ & numpy){

    std::stringstream state{};
    state << cpp_gen;

    std::string auxiliary_string{};
    py::list state_list{};

    for(unsigned int n = 0; n < 624; ++n){
        state >> auxiliary_string;
        state_list.append(std::stoul(auxiliary_string));
    }
    state >> auxiliary_string;
    unsigned int pos = std::stoul(auxiliary_string);

    py::object array = numpy.attr("array")(state_list,"uint32");
    py::dict state_dict("key"_a=array,"pos"_a=pos);
    py::dict d("bit_generator"_a="MT19937","state"_a=state_dict);
    py_gen.attr("__setstate__")(d);

}

void synchronize_py_to_cpp_state(std::mt19937 & cpp_gen,const py::object & py_gen) {

    py::object py_state = py_gen.attr("__getstate__")();
    py::object state_ = py_state["state"]["key"].attr("tolist")();
    auto pos_ = py_state["state"]["pos"].cast<unsigned int>();

    std::stringstream ss_state_;
    for(auto elem:state_){
        ss_state_ << elem.cast<unsigned int>() << " ";
    }
    ss_state_ << pos_;
    ss_state_ >> cpp_gen;
}
