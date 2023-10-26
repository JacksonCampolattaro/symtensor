#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
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

    } else if constexpr (Order == 4) {

        auto A = g4 * SymmetricTensor3f < 4 > ::CartesianPower(R);

        // Why doesn't R at factored into this term?
        SymmetricTensor3f<4> B{};
        B.at<X, X, Y, Y>() = 1;
        B.at<X, X, Z, Z>() = 1;
        B.at<Y, Y, Z, Z>() = 1;
        B += SymmetricTensor3f < 4 > ::Diagonal(glm::vec3{3, 3, 3});
        B *= g2;

        SymmetricTensor3f<4> C{};
        SymmetricTensor3f<2> R2 = SymmetricTensor3f < 2 > ::CartesianPower(R);
        C += 6.0f * SymmetricTensor3f < 4 > ::Diagonal(R2.diagonal());
        C.at<X, X, X, Y>() = 3 * R2.at<X, Y>();
        C.at<X, Y, Y, Y>() = 3 * R2.at<X, Y>();
        C.at<X, X, X, Z>() = 3 * R2.at<X, Z>();
        C.at<X, Z, Z, Z>() = 3 * R2.at<X, Z>();
        C.at<Y, Y, Y, Z>() = 3 * R2.at<Y, Z>();
        C.at<Y, Z, Z, Z>() = 3 * R2.at<Y, Z>();
        C.at<X, X, Y, Y>() = R2.at<X, X>() + R2.at<Y, Y>();
        C.at<X, X, Z, Z>() = R2.at<X, X>() + R2.at<Z, Z>();
        C.at<Y, Y, Z, Z>() = R2.at<Y, Y>() + R2.at<Z, Z>();
        C.at<X, X, Y, Z>() = R2.at<Y, Z>();
        C.at<X, Y, Y, Z>() = R2.at<X, Z>();
        C.at<X, Y, Z, Z>() = R2.at<X, Y>();
        C *= g3;

        return A + B + C;

    }
}

TEST_CASE("Multipole moment constructors", "[MultipoleMoment]") {

    REQUIRE(QuadrupoleMoment3f{} == QuadrupoleMoment3f{0, {0, 0, 0}, {0, 0, 0, 0, 0, 0}});

    REQUIRE(QuadrupoleMoment3f::FromPointMass(1.0f, glm::vec3{1, 2, 3}) ==
            QuadrupoleMoment3f{1, {1, 2, 3}, {1, 2, 3, 4, 6, 9}});

}

static glm::vec3 gravitationalAcceleration(std::vector<glm::vec4> particles, glm::vec3 position) {
    return std::transform_reduce(
            particles.begin(), particles.end(),
            glm::vec3{}, std::plus<>{},
            [&](auto particle) {
                auto R = position - glm::vec3{particle};
                return -R * (float) (particle.w / std::pow(glm::length(R), 3));
            }
    );
}

static glm::vec3 gravitationalAcceleration(glm::vec4 centerOfMass, glm::vec3 position) {
    auto R = position - glm::vec3{centerOfMass};
    return -R * (float) (centerOfMass.w / std::pow(glm::length(R), 3));
}

static glm::vec3 gravitationalAcceleration(QuadrupoleMoment3f quadrupole, glm::vec3 position) {
    auto R = position - to_glm(quadrupole.tensor<1>());
    return -R * (float) (quadrupole.scalar() / std::pow(glm::length(R), 3))
           + to_glm(D<3>(R, glm::length(R)) * quadrupole.tensor<2>() / 2.0f);
}

static glm::vec3 gravitationalAcceleration(OctupoleMoment3f octupole, glm::vec3 position) {
    auto R = position - to_glm(octupole.tensor<1>());
    return -R * (float) (octupole.scalar() / std::pow(glm::length(R), 3))
           + to_glm(D<3>(R, glm::length(R)) * octupole.tensor<2>() / 2.0f)
           - to_glm(D<4>(R, glm::length(R)) * octupole.tensor<3>() / 6.0f);
}

static float accelerationError(const glm::vec3 &trueAcceleration, const glm::vec3 &approximateAcceleration) {
    auto error = approximateAcceleration - trueAcceleration;
    return glm::length(error) / glm::length(trueAcceleration);
}

template<typename WorseApproximationFunction, typename BetterApproximationFunction>
void compareAccuracy(WorseApproximationFunction worseApproximationFunction,
                     BetterApproximationFunction betterApproximationFunction) {
    std::mt19937 generator{3};
    std::uniform_real_distribution<float> positionDistribution{-1.0f, 1.0f};
    std::uniform_real_distribution<float> massDistribution{0.1f, 1.0f};

    // Generate a handful of random particles
    std::vector<glm::vec4> particles{};
    for (int i = 0; i < 10; ++i)
        particles.emplace_back(
                positionDistribution(generator),
                positionDistribution(generator),
                positionDistribution(generator),
                massDistribution(generator)
        );

    // Generate both approximate representations of the particles
    auto worseApproximation = worseApproximationFunction(particles);
    auto betterApproximation = betterApproximationFunction(particles);

    // Generate some sample positions
    std::vector<glm::vec3> samplePositions;
    while (samplePositions.size() < 10000) {
        glm::vec3 position{
                positionDistribution(generator) * 10,
                positionDistribution(generator) * 10,
                positionDistribution(generator) * 10
        };
        if (glm::length(position) > 3.0f)
            samplePositions.push_back(position);
    }

    // Produce approximations
    std::vector<glm::vec3> trueAccelerations, worseApproximateAccelerations, betterApproximateAccelerations;
    std::transform(
            samplePositions.begin(), samplePositions.end(), std::back_inserter(trueAccelerations),
            [&](glm::vec3 position) { return gravitationalAcceleration(particles, position); }
    );
    std::transform(
            samplePositions.begin(), samplePositions.end(), std::back_inserter(worseApproximateAccelerations),
            [&](glm::vec3 position) { return gravitationalAcceleration(worseApproximation, position); }
    );
    std::transform(
            samplePositions.begin(), samplePositions.end(), std::back_inserter(betterApproximateAccelerations),
            [&](glm::vec3 position) { return gravitationalAcceleration(betterApproximation, position); }
    );

    // Compute errors for both approximations
    std::vector<float> worseErrors, betterErrors;
    std::transform(
            trueAccelerations.begin(), trueAccelerations.end(), worseApproximateAccelerations.begin(),
            std::back_inserter(worseErrors), accelerationError
    );
    std::transform(
            trueAccelerations.begin(), trueAccelerations.end(), betterApproximateAccelerations.begin(),
            std::back_inserter(betterErrors), accelerationError
    );

    // Max and mean error should be better for the better approximation
    CHECK(*std::max_element(worseErrors.begin(), worseErrors.end()) >=
          *std::max_element(betterErrors.begin(), betterErrors.end()));
    CHECK(std::reduce(worseErrors.begin(), worseErrors.end()) >
          std::reduce(betterErrors.begin(), betterErrors.end()));
    std::cout << *std::max_element(worseErrors.begin(), worseErrors.end()) << " "
              << *std::max_element(betterErrors.begin(), betterErrors.end()) << std::endl;
    std::cout << std::reduce(worseErrors.begin(), worseErrors.end()) << " "
              << std::reduce(betterErrors.begin(), betterErrors.end()) << std::endl;

}

TEST_CASE("Higher order approximations should be more accurate", "[MultipoleMoment]") {
    // todo: particles must be weighted by mass!

    auto computeCenterOfMass = [](const std::vector<glm::vec4> &particles) {
        glm::vec3 centerOfMass{};
        float totalMass = 0;
        for (auto &particle: particles) {
            totalMass += particle.w;
            centerOfMass += glm::vec3{particle} * particle.w;
        }
        centerOfMass /= totalMass;
        return glm::vec4{centerOfMass.x, centerOfMass.y, centerOfMass.z, totalMass};
    };
    auto computeQuadrupole = [](auto particles) {
        float totalMass = 0;
        return std::transform_reduce(
                particles.begin(), particles.end(),
                QuadrupoleMoment3f{}, std::plus<>{},
                [&](auto particle) {
                    totalMass += particle.w;
                    return QuadrupoleMoment3f::FromPointMass(particle.w, glm::vec3{particle}) * particle.w;
                }
        ) / totalMass;
    };
    auto computeOctupole = [](auto particles) {
        float totalMass = 0;
        return std::transform_reduce(
                particles.begin(), particles.end(),
                OctupoleMoment3f{}, std::plus<>{},
                [&](auto particle) {
                    totalMass += particle.w;
                    return OctupoleMoment3f::FromPointMass(particle.w, glm::vec3{particle}) * particle.w;
                }
        ) / totalMass;
    };

    compareAccuracy(computeCenterOfMass, computeQuadrupole);
    compareAccuracy(computeQuadrupole, computeOctupole);
}

