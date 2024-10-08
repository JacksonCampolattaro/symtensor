project(symtensor-benchmarks)

find_package(Catch2 3 QUIET)
if (Catch2_FOUND)
    message(STATUS "Using a packaged version of Catch2")
else ()
    message(STATUS "Catch2 (>3.0) could not be found, downloading instead")
    include(FetchContent)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.4.0)
    FetchContent_MakeAvailable(Catch2)
endif ()

if (NOT TARGET symtensor)
    find_package(symtensor REQUIRED)
endif ()

add_executable(
        benchmarks
        symmetricTensor.cpp
        multipole.cpp
        multipoleMoment.cpp
        gravity.cpp
)
target_link_libraries(benchmarks PRIVATE
        Catch2::Catch2WithMain
        symtensor::symtensor
)
target_compile_options(
        benchmarks PRIVATE
        $<$<CXX_COMPILER_ID:Clang>:-fassociative-math>
        $<$<CXX_COMPILER_ID:AppleClang>:-fassociative-math>
)
