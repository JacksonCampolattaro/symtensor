#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <symtensor/Multipole.h>
#include <symtensor/SymmetricTensor.h>

#include <iostream>
#include <numeric>

#include <glm/vec2.hpp>

using namespace symtensor;

TEST_CASE("Arithmetic operations with a scalar", "[Quadrupole]") {

    using Q = Quadrupole<SymmetricTensor3f<1>>;

    Q q{};
    q += 3;
    std::cout << q << std::endl;

    std::cout << Multipole<4, float, SymmetricTensor3f<1>>{} << std::endl;
}
