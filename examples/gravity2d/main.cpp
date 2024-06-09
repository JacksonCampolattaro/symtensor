#include <vector>
#include <iostream>

#include <symtensor/symtensor.h>
#include <symtensor/MultipoleMoment.h>
#include <glm/vec2.hpp>

int main() {

    auto points = std::vector<glm::vec2>{
            {0.0f, 4.0f},
            {1.0f, 3.0f},
            {2.0f, 2.0f},
            {3.0f, 1.0f},
            {4.0f, 0.0f}
    };

    for (const auto &p : points) {
        auto q = symtensor::QuadrupoleMoment2f::FromPosition(p);
        std::cout << p.x << " " << p.y << std::endl;
        std::cout << q << std::endl;
    }

}