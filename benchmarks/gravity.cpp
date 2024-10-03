#define GLM_FORCE_SWIZZLE

#include <random>

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <symtensor/Multipole.h>

#include "symtensor/gravity/direct.h"
#include "symtensor/gravity/einsum.h"
#include "symtensor/gravity/tensorlib.h"
#include "symtensor/glm.h"

using namespace symtensor;

TEST_CASE("benchmark: Multipole moment gravity approximation", "[Gravity]") {
    // todo
    //return gravity::einsum::derivative<3>(R);

    // Create some random particles
    std::mt19937 generator{0u};
    std::uniform_real_distribution<float> positionDistribution{-0.5f, 0.5f};
    std::uniform_real_distribution<float> massDistribution{0.1f, 1.0f};
    std::vector<glm::vec4> particles{};
    for (int i = 0; i < 32; ++i)
        particles.emplace_back(
                positionDistribution(generator),
                positionDistribution(generator),
                positionDistribution(generator),
                massDistribution(generator)
        );

    // Produce a multipole from the points
    float totalMass = 0;
    auto quadrupole = std::transform_reduce(
            particles.begin(), particles.end(),
            QuadrupoleMoment3f{}, std::plus<>{},
            [&](auto particle) {
                totalMass += particle.w;
                return QuadrupoleMoment3f::FromPosition(glm::vec3{particle}) * particle.w;
            }
    ) / totalMass;
    // todo: recenter on COM


    auto naive_acceleration = [](std::span<glm::vec4> particles, glm::vec3 position) {
        return std::transform_reduce(
                particles.begin(), particles.end(),
                glm::vec3{}, std::plus<>{},
                [&](const auto &q) {
                    auto R = q.xyz() - position.xyz();
                    auto r = glm::length(R) + 1e-7f;
                    return R * (q.w / (r * r * r));
                }
        );
    };

    // Calculate gravity the naive way
    BENCHMARK_ADVANCED("Naive Gravity")(Catch::Benchmark::Chronometer meter) {
            auto position = glm::vec3{
                    positionDistribution(generator) * 10,
                    positionDistribution(generator) * 10,
                    positionDistribution(generator) * 10,
            };
            meter.measure([&] { return naive_acceleration(particles, position); });
        };


    auto quadrupole_acceleration = [](QuadrupoleMoment3f quadrupole, glm::vec3 position) {
        auto R = position - to_glm(quadrupole.tensor<1>());
        auto derivative = symtensor::gravity::einsum::derivative<3>(R);
        return -R * (float) (quadrupole.scalar() / std::pow(glm::length(R), 3))
               + to_glm(derivative * quadrupole.tensor<2>() / 2.0f);
    };

    // Calculate gravity with the multipole
    BENCHMARK_ADVANCED("Quadrupole Gravity")(Catch::Benchmark::Chronometer meter) {
            auto position = glm::vec3{
                    positionDistribution(generator) * 10,
                    positionDistribution(generator) * 10,
                    positionDistribution(generator) * 10,
            };
            meter.measure([&] { return quadrupole_acceleration(quadrupole, position); });
        };

}
