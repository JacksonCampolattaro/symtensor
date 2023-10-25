#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>

#include <symtensor/Multipole.h>

using namespace symtensor;

TEST_CASE("benchmark: Multipole construction from a vector", "[Multipole]") {

    SymmetricTensor3f<1> vector{1, 2, 3};

    // Quadrupole
    {

        BENCHMARK("Quadrupole constructor") { return Quadrupole3f{vector}; };
        BENCHMARK("Quadrupole creation method") { return Quadrupole3f::FromVector(vector); };
        BENCHMARK("Quadrupole manual construction") {
                                                        return std::tuple<
                                                                SymmetricTensor3f<1>,
                                                                SymmetricTensor3f<2>
                                                        >{
                                                                vector,
                                                                SymmetricTensor3f<2>::CartesianPower(vector)
                                                        };
                                                    };
    }

    // Octupole
    {

        BENCHMARK("Octupole constructor") { return Octupole3f{vector}; };
        BENCHMARK("Octupole creation method") { return Octupole3f::FromVector(vector); };
        BENCHMARK("Octupole manual construction") {
                                                      return std::tuple<
                                                              SymmetricTensor3f<1>,
                                                              SymmetricTensor3f<2>,
                                                              SymmetricTensor3f<3>
                                                      >{
                                                              vector,
                                                              SymmetricTensor3f<2>::CartesianPower(vector),
                                                              SymmetricTensor3f<3>::CartesianPower(vector)
                                                      };
                                                  };
    }

    // Hexadecupole
    {

        BENCHMARK("Hexadecupole constructor") { return Hexadecupole3f{vector}; };
        BENCHMARK("Hexadecupole creation method") { return Hexadecupole3f::FromVector(vector); };
        BENCHMARK("Hexadecupole manual construction")
                    {
                        return std::tuple<
                                SymmetricTensor3f<1>,
                                SymmetricTensor3f<2>,
                                SymmetricTensor3f<3>,
                                SymmetricTensor3f<4>
                        >{
                                vector,
                                SymmetricTensor3f<2>::CartesianPower(vector),
                                SymmetricTensor3f<3>::CartesianPower(vector),
                                SymmetricTensor3f<4>::CartesianPower(vector)
                        };
                    };
        BENCHMARK("Hexadecupole cascading manual construction")
                    {
                        auto st2 = SymmetricTensor3f<2>::CartesianProduct(vector, vector);
                        auto st3 = SymmetricTensor3f<3>::CartesianProduct(st2, vector);
                        auto st4 = SymmetricTensor3f<4>::CartesianProduct(st3, vector);
                        return std::tuple<
                                SymmetricTensor3f<1>,
                                SymmetricTensor3f<2>,
                                SymmetricTensor3f<3>,
                                SymmetricTensor3f<4>
                        >{
                                vector,
                                st2,
                                st3,
                                st4
                        };
                    };
    }

    // Triacontadyupole
    {

        BENCHMARK("Triacontadyupole constructor") { return Triacontadyupole3f{vector}; };
        BENCHMARK("Triacontadyupole creation method") { return Triacontadyupole3f::FromVector(vector); };
        BENCHMARK("Triacontadyupole manual construction")
                    {
                        return std::tuple<
                                SymmetricTensor3f<1>,
                                SymmetricTensor3f<2>,
                                SymmetricTensor3f<3>,
                                SymmetricTensor3f<4>,
                                SymmetricTensor3f<5>
                        >{
                                vector,
                                SymmetricTensor3f<2>::CartesianPower(vector),
                                SymmetricTensor3f<3>::CartesianPower(vector),
                                SymmetricTensor3f<4>::CartesianPower(vector),
                                SymmetricTensor3f<5>::CartesianPower(vector)
                        };
                    };
        BENCHMARK("Triacontadyupole cascading manual construction")
                    {
                        auto st2 = SymmetricTensor3f<2>::CartesianProduct(vector, vector);
                        auto st3 = SymmetricTensor3f<3>::CartesianProduct(st2, vector);
                        auto st4 = SymmetricTensor3f<4>::CartesianProduct(st3, vector);
                        auto st5 = SymmetricTensor3f<5>::CartesianProduct(st4, vector);
                        return std::tuple<
                                SymmetricTensor3f<1>,
                                SymmetricTensor3f<2>,
                                SymmetricTensor3f<3>,
                                SymmetricTensor3f<4>,
                                SymmetricTensor3f<5>
                        >{
                                vector,
                                st2,
                                st3,
                                st4,
                                st5
                        };
                    };
    }

}
