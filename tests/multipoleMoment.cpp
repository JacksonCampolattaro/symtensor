#include <catch2/catch_test_macros.hpp>
#include <random>

#include <symtensor/MultipoleMoment.h>
#include <symtensor/glm.h>

#include <iostream>

#define GLM_FORCE_SWIZZLE

#include <glm/vec4.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace symtensor;

// todo: this needs to be regularized -- there must be a pattern!
template<std::size_t Order>
[[gnu::always_inline]] inline auto D(const glm::vec3 &R, float r) {

#if (__cpp_using_enum && !__clang__) || (__clang_major__ > 15)
    using
    enum SymmetricTensor3f<1>::Index;
#else
    using Index<3>::X;
    using Index<3>::Y;
    using Index<3>::Z;
#endif

    float f1 = -1.0f / pow<2>(r);
    float f2 = 3.0f / pow<3>(r);
    float f3 = -15.0f / pow<4>(r);
    float f4 = 105.0f / pow<5>(r);
    float f5 = -945.0f / pow<6>(r);

    float g1 = f1 / r;
    float g2 = f2 / pow<2>(r);
    float g3 = f3 / pow<3>(r);
    float g4 = f4 / pow<4>(r);
    float g5 = f5 / pow<5>(r);

    if constexpr (Order == 1) {

        return g1 * R;

    } else if constexpr (Order == 2) {

        return (g2 * SymmetricTensor3f < 2 > ::CartesianPower(R)) + (g1 * SymmetricTensor3f < 2 > ::Identity());

    } else if constexpr (Order == 3) {

        auto A = g3 * SymmetricTensor3f < 3 > ::CartesianPower(R);
        auto B = SymmetricTensor3f < 3 > {};
        B.at<X, Y, Y>() = R.x;
        B.at<X, Z, Z>() = R.x;
        B.at<Y, X, X>() = R.y;
        B.at<Y, Z, Z>() = R.y;
        B.at<Z, X, X>() = R.z;
        B.at<Z, Y, Y>() = R.z;
        B += 3 * SymmetricTensor3f < 3 > ::Diagonal(R);
        B *= g2;
        return A + B;

    }
}

TEST_CASE("Multipole moment constructors", "[MultipoleMoment]") {

    REQUIRE(QuadrupoleMoment3f{} == QuadrupoleMoment3f{0, {0, 0, 0}, {0, 0, 0, 0, 0, 0}});

    REQUIRE(QuadrupoleMoment3f::FromPointMass(1.0f, glm::vec3{1, 2, 3}) ==
            QuadrupoleMoment3f{1, {1, 2, 3}, {1, 2, 3, 4, 6, 9}});

}

TEST_CASE("Quadrupole moment should be more accurate than center-of-mass", "[MultipoleMoment]") {
    std::mt19937 generator{0};
    std::uniform_real_distribution<float> positionDistribution{-1.0f, 1.0f};
    std::uniform_real_distribution<float> massDistribution{0.1f, 1.0f};

    // Generate a handful of random particles
    std::vector<glm::vec4> particles{};
    for (int i = 0; i < 100; ++i)
        particles.emplace_back(
                positionDistribution(generator),
                positionDistribution(generator),
                positionDistribution(generator),
                massDistribution(generator)
        );

    // Create a center-of-mass approximation
    glm::vec4 centerOfMass = std::reduce(particles.begin(), particles.end());
    centerOfMass.x /= (float) particles.size();
    centerOfMass.y /= (float) particles.size();
    centerOfMass.z /= (float) particles.size();

    // Create a quadrupole approximation
    auto quadrupoleMoment = std::transform_reduce(
            particles.begin(), particles.end(),
            QuadrupoleMoment3f{}, std::plus<>{},
            [](auto particle) {
                return QuadrupoleMoment3f::FromPointMass(particle.w, glm::vec3{particle});
            }
    ) / (float) particles.size();

    // We'll track the error of the two approximations
    std::vector<float> centerOfMassErrors, quadrupoleErrors;

    // Sample several locations
    for (int i = 0; i < 100; ++i) {
        glm::vec3 position{
                positionDistribution(generator) * 100,
                positionDistribution(generator) * 100,
                positionDistribution(generator) * 100
        };

        // Skip positions which are too close
        if (glm::length(position) < 5.0f)
            continue;

        // Compute the true acceleration
        auto exactAcceleration = std::transform_reduce(
                particles.begin(), particles.end(),
                glm::vec3{}, std::plus<>{},
                [&](auto particle) {
                    auto R = position - glm::vec3{particle};
                    return -R * (float) (particle.w / std::pow(glm::length(R), 3));
                }
        );

        // Compute the center of mass approximation
        auto R = position - glm::vec3{centerOfMass};
        auto centerOfMassApproximation = -R * (float) (centerOfMass.w / std::pow(glm::length(R), 3));

        // Compute the quadrupole approximation
        auto quadrupoleApproximation = -R * (float) (quadrupoleMoment.scalar() / std::pow(glm::length(R), 3))
                                       + to_glm(D<3>(R, glm::length(R)) * quadrupoleMoment.tensor<2>() / 2.0f);

        // Determine error
        auto centerOfMassError = (centerOfMassApproximation - exactAcceleration) / glm::length(exactAcceleration);
        auto quadrupoleError = (quadrupoleApproximation - exactAcceleration) / glm::length(exactAcceleration);
        centerOfMassErrors.emplace_back(glm::length(centerOfMassError));
        quadrupoleErrors.emplace_back(glm::length(quadrupoleError));

    }

    // The quadrupole should be more accurate overall than the center-of-mass approximation
    REQUIRE(*std::max_element(centerOfMassErrors.begin(), centerOfMassErrors.end()) >
            *std::max_element(quadrupoleErrors.begin(), quadrupoleErrors.end()));

}

