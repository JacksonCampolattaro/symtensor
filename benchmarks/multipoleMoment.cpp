#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <symtensor/Multipole.h>
#include <symtensor/gravity.h>

using namespace symtensor;

TEST_CASE("benchmark: Multipole moment gravity approximation", "[MultipoleMoment]") {
    // todo
}

TEST_CASE("benchmark: Gravity derivatives construction", "[Gravity]") {
    auto R = glm::vec3{1.0, 2.0, 3.0};


    constexpr std::array<int, 5> input = {1, 2, 2, 3, 1};
    constexpr auto result = filter<input, [&](auto v) constexpr { return v != 3; }>();

    // Print the result (for non-constexpr context)
    for (const auto &value: result) {
        std::cout << value << std::endl;
    }

    CHECK((gravity::D<2>(R) - gravity::derivative<2>(R)).norm() < 1e-7);
    CHECK((gravity::D<3>(R) - gravity::derivative<3>(R)).norm() < 1e-7);
    CHECK((gravity::D<4>(R) - gravity::derivative<4>(R)).norm() < 1e-7);

//    BENCHMARK("D' Construction") { return gravity::D<1>(R); };
//    BENCHMARK("D' Construction *") { return gravity::derivative<1>(R); };
//    BENCHMARK("D'' Construction") { return gravity::D<2>(R); };
//    BENCHMARK("D'' Construction *") { return gravity::derivative<2>(R); };
//    BENCHMARK("D''' Construction") { return gravity::D<3>(R); };
//    BENCHMARK("D''' Construction *") { return gravity::derivative<3>(R); };
//    BENCHMARK("D'''' Construction") { return gravity::D<4>(R); };
//    BENCHMARK("D'''' Construction *") { return gravity::derivative<4>(R); };

    BENCHMARK("D1-4 Construction") { return gravity::Ds<4>(R); };
    BENCHMARK("D1-4 Construction (improved)") { return gravity::derivatives<4>(R); };
}
