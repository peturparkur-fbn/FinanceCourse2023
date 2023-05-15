cmake_minimum_required(VERSION 3.25)
project(StructuredBinomPricing)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_COMPILER /opt/homebrew/bin/gcc-13)

#add_library(structures BinomPricer.cpp BinomPricer.h Options.h)
add_executable(StructuredBinomPricing main.cpp)

target_link_libraries(StructuredBinomPricing)
