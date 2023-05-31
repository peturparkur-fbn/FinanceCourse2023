cmake_minimum_required(VERSION 3.25)
project(Structured)

set(CMAKE_CXX_STANDARD 23)

add_library(Structured BinomPricer.h BinomPricer.cpp Options.cpp Options.h MonteCarlo.cpp Integration.hpp)