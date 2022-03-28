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
    std::stringstream state{};
    state << gen;
    //std::cout << state.str() << std::endl;

    std::mt19937 gen_new(0);

    //state >> gen_new;
    //std::cout << (gen == gen_new) << std::endl;

    // Create a generator in python
    auto numpy = py::module_::import("numpy");
    auto numpy_random = py::module_::import("numpy.random");
    auto py_engine = numpy_random.attr("MT19937")();
    auto py_gen = numpy_random.attr("Generator")(py_engine);

//    synchronize_cpp_to_py_state(gen,py_gen,numpy);
//    std::cout << gen << std::endl;
//    py::print(py_gen.attr("__getstate__")());

    synchronize_py_to_cpp_state(gen,py_gen);
    std::cout << gen << std::endl;
    py::print(py_gen.attr("__getstate__")());

}

void synchronize_cpp_to_py_state(const std::mt19937 & cpp_gen, py::object & py_gen, py::module_ & numpy){

    std::stringstream state{};
    state << cpp_gen;
    std::string s{};
    py::list state_list{};
//    needed ?
//    state.clear();
//    state.seekg(0);
    for(unsigned int n = 0; n < 624; ++n){
        state >> s;
        state_list.append(std::stoul(s));
    }
    auto array = numpy.attr("array")(state_list,"uint32");
    state >> s;
    unsigned int pos = std::stoul(s);
    py::dict state_dict("key"_a=array,"pos"_a=pos);
    py::dict d("bit_generator"_a="MT19937","state"_a=state_dict);
    py_gen.attr("__setstate__")(d);
}

void synchronize_py_to_cpp_state(std::mt19937 & cpp_gen,const py::object & py_gen) {
    auto py_state = py_gen.attr("__getstate__")();
    auto state_ = py_state["state"]["key"];
    auto pos_ = py_state["state"]["pos"];
    auto list_state_ = state_.attr("tolist")();
    std::stringstream ss_state_;
    for(auto elem:list_state_){
        ss_state_ << elem.cast<unsigned int>() << " ";
    }
    ss_state_ << pos_.cast<unsigned int>();
    ss_state_ >> cpp_gen;
}
