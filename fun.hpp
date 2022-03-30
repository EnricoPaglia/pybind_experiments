#include <random>
#include <sstream>
#include <string>

#include <pybind11/pybind11.h>


namespace py = pybind11;
using namespace py::literals;

void synchronize_cpp_to_py_state(const std::mt19937 &cpp_gen, py::object &py_gen) {
    // TODO: Check if it's more efficient to pass py::module_ &numpy as an argument and create it only once in main

    auto numpy = py::module_::import("numpy");
    std::stringstream state{};
    state << cpp_gen;

    std::string aux_string{};
    py::list state_list{};

    for (unsigned int n = 0; n < 624; ++n) {
        state >> aux_string;
        state_list.append(std::stoul(aux_string));
    }
    state >> aux_string;
    unsigned int pos = std::stoul(aux_string);

    py::object array = numpy.attr("array")(state_list, "uint32");
    py::dict state_dict("key"_a = array, "pos"_a = pos);
    py::dict d("bit_generator"_a = "MT19937", "state"_a = state_dict);
    py_gen.attr("__setstate__")(d);

}

void synchronize_py_to_cpp_state(std::mt19937 &cpp_gen, const py::object &py_gen) {

    py::object py_state = py_gen.attr("__getstate__")();
    py::object state_ = py_state["state"]["key"].attr("tolist")();
    auto pos_ = py_state["state"]["pos"].cast<unsigned int>();

    std::stringstream ss_state_;
    for (auto elem: state_) {
        ss_state_ << elem.cast<unsigned int>() << " ";
    }
    ss_state_ << pos_;
    ss_state_ >> cpp_gen;
}
