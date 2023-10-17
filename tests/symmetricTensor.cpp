#include <catch2/catch_test_macros.hpp>

#include <symtensor/SymmetricTensor.h>
#include <symtensor/glm.h>

#include <glm/matrix.hpp>
#include <glm/gtx/io.hpp>

#include <iostream>
#include <numeric>

using namespace symtensor;

// Workaround for compiler defect DR2621 in Clang 15
// https://reviews.llvm.org/D134283
#if (__cpp_using_enum && !__clang__) || (__clang_major__ > 15)
using enum SymmetricTensor3f<1>::Index;
#else
using Index<3>::X;
using Index<3>::Y;
using Index<3>::Z;
#endif


TEST_CASE("Member access to a 3x3 symmetric tensor", "[SymmetricTensor]") {

    using Quadrupole = SymmetricTensor3f<2>;

    // Construct a quadrupole with known values
    constexpr Quadrupole q{0, 1, 2, 3, 4, 5};

    CHECK(q[{X, X}] == 0);

    CHECK(q[{X, Y}] == 1);
    CHECK(q[{Y, X}] == 1);

    CHECK(q[{X, Z}] == 2);
    CHECK(q[{Z, X}] == 2);

    CHECK(q[{Y, Y}] == 3);

    CHECK(q[{Y, Z}] == 4);
    CHECK(q[{Z, Y}] == 4);

    CHECK(q[{Z, Z}] == 5);
}

TEST_CASE("Member access to a 3x3x3 symmetric tensor", "[SymmetricTensor]") {

    using Octupole = SymmetricTensor3f<3>;

    // Construct a quadrupole with known values
    Octupole o{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    CHECK(o[{X, X, X}] == 0);

    CHECK(o[{X, X, Y}] == 1);
    CHECK(o[{X, Y, X}] == 1);

    CHECK(o[{X, X, Z}] == 2);
    CHECK(o[{X, Z, X}] == 2);

    CHECK(o[{X, Y, Y}] == 3);
    CHECK(o[{Y, X, Y}] == 3);

    CHECK(o[{X, Y, Z}] == 4);
    CHECK(o[{X, Z, Y}] == 4);

    CHECK(o[{X, Z, Z}] == 5);
    CHECK(o[{Z, X, Z}] == 5);

    CHECK(o[{Y, Y, Y}] == 6);

    CHECK(o[{Y, Y, Z}] == 7);
    CHECK(o[{Y, Z, Y}] == 7);

    CHECK(o[{Y, Z, Z}] == 8);
    CHECK(o[{Z, Y, Z}] == 8);

    CHECK(o[{Z, Z, Z}] == 9);

}

TEST_CASE("Member access to a 3x3x3x3 symmetric tensor", "[SymmetricTensor]") {

    using Hexadecupole = SymmetricTensor3f<4>;

    // Construct a quadrupole with known values
    Hexadecupole h{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

    CHECK(h[{X, X, X, X}] == 0);

    CHECK(h[{X, X, X, Y}] == 1);
    CHECK(h[{X, X, Y, X}] == 1);

    CHECK(h[{X, X, X, Z}] == 2);
    CHECK(h[{X, X, Z, X}] == 2);

    CHECK(h[{X, X, Y, Y}] == 3);
    CHECK(h[{X, Y, X, Y}] == 3);

    CHECK(h[{X, X, Y, Z}] == 4);
    CHECK(h[{X, X, Z, Y}] == 4);

    CHECK(h[{X, X, Z, Z}] == 5);
    CHECK(h[{X, Z, X, Z}] == 5);

    CHECK(h[{X, Y, Y, Y}] == 6);

    CHECK(h[{X, Y, Y, Z}] == 7);
    CHECK(h[{X, Y, Z, Y}] == 7);

    CHECK(h[{X, Y, Z, Z}] == 8);
    CHECK(h[{X, Z, Y, Z}] == 8);

    CHECK(h[{X, Z, Z, Z}] == 9);

    CHECK(h[{Y, Y, Y, Y}] == 10);

    CHECK(h[{Y, Y, Y, Z}] == 11);
    CHECK(h[{Y, Y, Z, Y}] == 11);

    CHECK(h[{Y, Y, Z, Z}] == 12);
    CHECK(h[{Y, Z, Y, Z}] == 12);

    CHECK(h[{Y, Z, Z, Z}] == 13);
    CHECK(h[{Z, Y, Z, Z}] == 13);

    CHECK(h[{Z, Z, Z, Z}] == 14);

}

TEST_CASE("Symmetric tensor constructors", "[SymmetricTensor]") {

    CHECK(SymmetricTensor2f<1>::Identity() == SymmetricTensor2f<1>{1, 1});
    CHECK(SymmetricTensor2f<2>::Identity() == SymmetricTensor2f<2>{1, 0, 1});
    CHECK(SymmetricTensor2f<3>::Identity() == SymmetricTensor2f<3>{1, 0, 0, 1});
    CHECK(SymmetricTensor2f<4>::Identity() == SymmetricTensor2f<4>{1, 0, 0, 0, 1});
    CHECK(SymmetricTensor2f<5>::Identity() == SymmetricTensor2f<5>{1, 0, 0, 0, 0, 1});

    CHECK(SymmetricTensor3f<1>::Identity() == SymmetricTensor3f<1>{1, 1, 1});
    CHECK(SymmetricTensor3f<2>::Identity() == SymmetricTensor3f<2>{1, 0, 0, 1, 0, 1});
    CHECK(SymmetricTensor3f<3>::Identity() == SymmetricTensor3f<3>{1, 0, 0, 0, 0, 0, 1, 0, 0, 1});
    CHECK(SymmetricTensor3f<4>::Identity() == SymmetricTensor3f<4>{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1});

    CHECK(SymmetricTensor2f<1>::Ones() == SymmetricTensor2f<1>{1, 1});
    CHECK(SymmetricTensor2f<2>::Ones() == SymmetricTensor2f<2>{1, 1, 1});
    CHECK(SymmetricTensor3f<2>::Ones() == SymmetricTensor3f<2>{1, 1, 1, 1, 1, 1});
    CHECK(SymmetricTensor3f<3>::Ones() == SymmetricTensor3f<3>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
    CHECK(SymmetricTensor3f<4>::Ones() == SymmetricTensor3f<4>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});

    CHECK(SymmetricTensor2f<1>::Diagonal(glm::vec2{1, 2}) == SymmetricTensor2f<1>{1, 2});
    CHECK(SymmetricTensor2f<2>::Diagonal(glm::vec2{1, 2}) == SymmetricTensor2f<2>{1, 0, 2});
    CHECK(SymmetricTensor3f<2>::Diagonal(glm::vec3{1, 2, 3}) == SymmetricTensor3f<2>{1, 0, 0, 2, 0, 3});
}

TEST_CASE("Symmetric tensor comparison", "[SymmetricTensor]") {

    SymmetricTensor3f<1> s1a{0, 1, 2};
    SymmetricTensor3f<1> s1b{};
    CHECK(s1a == s1a);
    CHECK(s1a != s1b);

    SymmetricTensor3f<2> s2a{0, 1, 2, 3, 4, 5};
    SymmetricTensor3f<2> s2b{};
    CHECK(s2a == s2a);
    CHECK(s2a != s2b);

    SymmetricTensor3f<3> s3a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    SymmetricTensor3f<3> s3b{};
    CHECK(s3a == s3a);
    CHECK(s3a != s3b);

}

TEST_CASE("Symmetric tensor arithmetic", "[SymmetricTensor]") {

    SymmetricTensor3f<3> s3a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    SymmetricTensor3f<3> s3b = s3a + s3a;
    SymmetricTensor3f<3> s3c = s3a + s3b;

    CHECK(s3b[{X, X, Y}] == 2);
    CHECK(s3c[{X, X, Y}] == 3);

    s3c -= s3b;
    CHECK(s3c == s3a);

    s3a += s3a;
    CHECK(s3a == s3b);

    std::vector<SymmetricTensor3f<2>> identities{
            SymmetricTensor3f < 2 > ::Identity(),
            SymmetricTensor3f < 2 > ::Identity(),
            SymmetricTensor3f < 2 > ::Identity(),
            SymmetricTensor3f < 2 > ::Identity(),
            SymmetricTensor3f < 2 > ::Identity()
    };
    auto identity5 = std::reduce(identities.begin(), identities.end());
    CHECK(identity5 == SymmetricTensor3f < 2 > ::Identity() * 5);
}


TEST_CASE("Symmetric tensor index format conversion", "[SymmetricTensor]") {

    {
        using Quadrupole = SymmetricTensor3f<2>;

        CHECK(Quadrupole::dimensionalIndices(0) == std::array<Quadrupole::Index, 2>{X, X});
        CHECK(Quadrupole::dimensionalIndices(1) == std::array<Quadrupole::Index, 2>{X, Y});
        CHECK(Quadrupole::dimensionalIndices(2) == std::array<Quadrupole::Index, 2>{X, Z});
        CHECK(Quadrupole::dimensionalIndices(3) == std::array<Quadrupole::Index, 2>{Y, Y});
        CHECK(Quadrupole::dimensionalIndices(4) == std::array<Quadrupole::Index, 2>{Y, Z});
        CHECK(Quadrupole::dimensionalIndices(5) == std::array<Quadrupole::Index, 2>{Z, Z});
    }

    // 3D
    {
        using Octupole = SymmetricTensor3f<3>;

        CHECK(Octupole::dimensionalIndices(0) == std::array<Octupole::Index, 3>{X, X, X});
        CHECK(Octupole::dimensionalIndices(1) == std::array<Octupole::Index, 3>{X, X, Y});
        CHECK(Octupole::dimensionalIndices(2) == std::array<Octupole::Index, 3>{X, X, Z});
        CHECK(Octupole::dimensionalIndices(3) == std::array<Octupole::Index, 3>{X, Y, Y});
        CHECK(Octupole::dimensionalIndices(4) == std::array<Octupole::Index, 3>{X, Y, Z});
        CHECK(Octupole::dimensionalIndices(5) == std::array<Octupole::Index, 3>{X, Z, Z});
        CHECK(Octupole::dimensionalIndices(6) == std::array<Octupole::Index, 3>{Y, Y, Y});
        CHECK(Octupole::dimensionalIndices(7) == std::array<Octupole::Index, 3>{Y, Y, Z});
        CHECK(Octupole::dimensionalIndices(8) == std::array<Octupole::Index, 3>{Y, Z, Z});
        CHECK(Octupole::dimensionalIndices(9) == std::array<Octupole::Index, 3>{Z, Z, Z});
    }

    // 4D
    {
        using Hexadecupole = SymmetricTensor3f<4>;

        CHECK(Hexadecupole::dimensionalIndices(0) == std::array<Hexadecupole::Index, 4>{X, X, X, X});
        CHECK(Hexadecupole::dimensionalIndices(1) == std::array<Hexadecupole::Index, 4>{X, X, X, Y});
        CHECK(Hexadecupole::dimensionalIndices(2) == std::array<Hexadecupole::Index, 4>{X, X, X, Z});
        CHECK(Hexadecupole::dimensionalIndices(3) == std::array<Hexadecupole::Index, 4>{X, X, Y, Y});
        CHECK(Hexadecupole::dimensionalIndices(4) == std::array<Hexadecupole::Index, 4>{X, X, Y, Z});
        CHECK(Hexadecupole::dimensionalIndices(5) == std::array<Hexadecupole::Index, 4>{X, X, Z, Z});
        CHECK(Hexadecupole::dimensionalIndices(6) == std::array<Hexadecupole::Index, 4>{X, Y, Y, Y});
        CHECK(Hexadecupole::dimensionalIndices(7) == std::array<Hexadecupole::Index, 4>{X, Y, Y, Z});
        CHECK(Hexadecupole::dimensionalIndices(8) == std::array<Hexadecupole::Index, 4>{X, Y, Z, Z});
        CHECK(Hexadecupole::dimensionalIndices(9) == std::array<Hexadecupole::Index, 4>{X, Z, Z, Z});
        CHECK(Hexadecupole::dimensionalIndices(10) == std::array<Hexadecupole::Index, 4>{Y, Y, Y, Y});
        CHECK(Hexadecupole::dimensionalIndices(11) == std::array<Hexadecupole::Index, 4>{Y, Y, Y, Z});
        CHECK(Hexadecupole::dimensionalIndices(12) == std::array<Hexadecupole::Index, 4>{Y, Y, Z, Z});
        CHECK(Hexadecupole::dimensionalIndices(13) == std::array<Hexadecupole::Index, 4>{Y, Z, Z, Z});
        CHECK(Hexadecupole::dimensionalIndices(14) == std::array<Hexadecupole::Index, 4>{Z, Z, Z, Z});
    }

}

TEST_CASE("Symmetric tensor initialization with an expression", "[SymmetricTensor]") {

    auto ones = SymmetricTensor3f < 3 > ::NullaryExpression([]([[maybe_unused]] auto _) {
        return 1;
    });
    REQUIRE(ones == SymmetricTensor3f < 3 > {1, 1, 1, 1, 1, 1, 1, 1, 1, 1});

    int i = 0;
    auto sequence = SymmetricTensor3f < 3 > ::NullaryExpression([&]([[maybe_unused]] auto _) {
        return i++;
    });
    REQUIRE(sequence == SymmetricTensor3f < 3 > {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

    auto sumOfDimensions = SymmetricTensor3f < 3 > ::NullaryExpression([&](auto dimensions) {
        return (float) dimensions[0] + (float) dimensions[1] + (float) dimensions[2];
    });
    REQUIRE(sumOfDimensions == SymmetricTensor3f < 3 > {0, 1, 2, 2, 3, 4, 3, 4, 5, 6});

}

TEST_CASE("Trace of tensors of different sizes", "[SymmetricTensor]") {

    REQUIRE(SymmetricTensor3f<1>::Identity().trace() == 3);
    REQUIRE(SymmetricTensor3f<2>::Identity().trace() == 3);
    REQUIRE(SymmetricTensor3f<3>::Identity().trace() == 3);
    REQUIRE(SymmetricTensor3f<4>::Identity().trace() == 3);
    REQUIRE(SymmetricTensor3f<5>::Identity().trace() == 3);
    REQUIRE(SymmetricTensor3f<1>{0, 1, 2}.trace() == 3);
    REQUIRE(SymmetricTensor3f<2>{0, 1, 2, 3, 4, 5}.trace() == 8);
    REQUIRE(SymmetricTensor3f<3>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}.trace() == 15);
}

TEST_CASE("Symmetric tensor product with a vector", "[SymmetricTensor]") {

    float v = 5;
    std::reference_wrapper<float> rv{v};
    std::reference_wrapper<std::reference_wrapper<float>> rrv{rv};
    REQUIRE(v == rrv.get());

    auto vec2 = glm::vec2{1, 2};
    auto svec2 = SymmetricTensor2f<1>{1, 2};
    REQUIRE(to_glm(SymmetricTensor2f<2>::CartesianPower(svec2)) == glm::outerProduct(vec2, vec2));
    // todo
    //auto identity2x2 = glm::mat2x2{};
    //REQUIRE(SymmetricTensor2f<2>::Identity() * glm::vec2{1, 2} == glm::outerProduct(identity2x2 * glm::transpose(vec2)));
    //    REQUIRE(SymmetricTensor2f<2>::Identity() * glm::vec2{1, 2} ==
    //            SymmetricTensor2f<3>{1, 0, 1, 2});
    //
    //    auto identity3x3 = glm::mat3x3{};
    //    auto vec3 = glm::vec3{1, 2, 3};
    //    REQUIRE(SymmetricTensor3f<2>::Identity() * glm::vec3{0, 1, 2} ==
    //            SymmetricTensor3f<3>{0, 0, 0, 0, 0, 0, 1, 0, 1, 2});
}
