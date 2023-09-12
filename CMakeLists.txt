
cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 20)

project(symtensor CXX)

# Options
option(SYMTENSOR_BUILD_TESTS "Build tests" ON)

# Header only library
add_library(symtensor INTERFACE)
target_include_directories(symtensor INTERFACE include)
add_library(symtensor::symtensor ALIAS symtensor)

if (SYMTENSOR_BUILD_TESTS)
    add_subdirectory(tests)
endif ()
