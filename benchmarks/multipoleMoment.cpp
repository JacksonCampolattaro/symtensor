#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <symtensor/Multipole.h>

#include "symtensor/gravity/direct.h"
#include "symtensor/gravity/einsum.h"
#include "symtensor/gravity/tensorlib.h"

using namespace symtensor;

TEST_CASE("benchmark: Gravity derivatives construction", "[Gravity]") {
    auto R = glm::vec3{1.0, 2.0, 3.0};

    // Einsum must match direct form
    CHECK((gravity::direct::derivative<2>(R) - gravity::einsum::derivative<2>(R)).norm() < 1e-7);
    CHECK((gravity::direct::derivative<3>(R) - gravity::einsum::derivative<3>(R)).norm() < 1e-7);
    CHECK((gravity::direct::derivative<4>(R) - gravity::einsum::derivative<4>(R)).norm() < 1e-7);
    // CHECK((gravity::D<5>(R) - gravity::derivative<5>(R)).norm() < 1e-7); // todo: unimplemented

    // Tensorlib must match direct form
    CHECK((gravity::direct::derivative<2>(R) - gravity::tensorlib::derivative<2>(R)).norm() < 1e-7);
    CHECK((gravity::direct::derivative<3>(R) - gravity::tensorlib::derivative<3>(R)).norm() < 1e-7);
    CHECK((gravity::direct::derivative<4>(R) - gravity::tensorlib::derivative<4>(R)).norm() < 1e-7);
    CHECK((gravity::direct::derivative<5>(R) - gravity::tensorlib::derivative<5>(R)).norm() < 1e-7);

    BENCHMARK("D' (direct)") { return gravity::direct::derivative<1>(R); };
    BENCHMARK("D' (einsum)") { return gravity::einsum::derivative<1>(R); };
    BENCHMARK("D' (tensorlib)") { return gravity::tensorlib::derivative<1>(R); };
    BENCHMARK("D'' (direct)") { return gravity::direct::derivative<2>(R); };
    BENCHMARK("D'' (einsum)") { return gravity::einsum::derivative<2>(R); };
    BENCHMARK("D'' (tensorlib)") { return gravity::tensorlib::derivative<2>(R); };
    BENCHMARK("D''' (direct)") { return gravity::direct::derivative<3>(R); };
    BENCHMARK("D''' (einsum)") { return gravity::einsum::derivative<3>(R); };
    BENCHMARK("D''' (tensorlib)") { return gravity::tensorlib::derivative<3>(R); };
    BENCHMARK("D'''' (direct)") { return gravity::direct::derivative<4>(R); };
    BENCHMARK("D'''' (einsum)") { return gravity::einsum::derivative<4>(R); };
    BENCHMARK("D'''' (tensorlib)") { return gravity::tensorlib::derivative<4>(R); };

    BENCHMARK("D' - D'''' (direct)") { return gravity::direct::derivatives<4>(R); };
    BENCHMARK("D' - D'''' (einsum)") { return gravity::einsum::derivatives<4>(R); };
    BENCHMARK("D' - D'''' (tensorlib)") { return gravity::tensorlib::derivatives<4>(R); };

    BENCHMARK("D' - D''''' (direct)") { return gravity::direct::derivatives<5>(R); };
    //BENCHMARK("D' - D''''' (einsum)") { return gravity::einsum::derivatives<5>(R); }; // todo: unimplemented
    BENCHMARK("D' - D''''' (tensorlib)") { return gravity::tensorlib::derivatives<5>(R); };

    //    BENCHMARK("D1-5 Construction (direct)") { return gravity::direct::derivatives<5>(R); };
    //    BENCHMARK("D1-5 Construction (tensorlib)") { return gravity::tensorlib::derivatives<5>(R); };
}
