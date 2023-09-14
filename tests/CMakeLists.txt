project(symtensor-tests)

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

add_executable(tests symmetricTensor3.cpp
        util.cpp
        symmetricTensor.cpp)
target_link_libraries(tests PRIVATE
        Catch2::Catch2WithMain
        symtensor::symtensor
)
