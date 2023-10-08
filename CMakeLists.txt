
cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)

project(symtensor CXX)

# Options
option(SYMTENSOR_BUILD_TESTS "Build tests" ON)
option(SYMTENSOR_BUILD_BENCHMARKS "Build benchmarks" ON)
option(SYMTENSOR_BUILD_DOCUMENTATION "Build documentation" ON)

# Dependencies
find_package(glm QUIET) # fixme: version specification doesn't seem to work in CMake 3.24!
if (glm_FOUND)
    message(STATUS "Using a packaged version of glm")
else ()
    message(STATUS "glm could not be found, downloading instead")
    include(FetchContent)
    FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG 0.9.9.8)
    FetchContent_MakeAvailable(glm)
endif ()

# Header only library
add_library(symtensor INTERFACE)
target_include_directories(symtensor INTERFACE include)
target_link_libraries(symtensor INTERFACE glm::glm)
add_library(symtensor::symtensor ALIAS symtensor)

# Build tests if requested
if (SYMTENSOR_BUILD_TESTS)
    message("Unit tests enabled")
    enable_testing()
    add_subdirectory(tests)
endif ()
if (SYMTENSOR_BUILD_BENCHMARKS)
    message("Benchmarks enabled")
    add_subdirectory(benchmarks)
endif ()
if (SYMTENSOR_BUILD_DOCUMENTATION)
    message("Documentation enabled")
    add_subdirectory(doc)
endif ()
