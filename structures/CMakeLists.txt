cmake_minimum_required(VERSION 3.25)
project(structures)

set(CMAKE_CXX_STANDARD 23)

add_library(structures BinomPricer.h Options.h)
