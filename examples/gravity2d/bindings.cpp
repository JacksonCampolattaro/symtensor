//#include <vector>
//#include <iostream>
//
//#include <symtensor/symtensor.h>
//#include <symtensor/MultipoleMoment.h>
//#include <glm/vec2.hpp>
//
//int main() {
//
//    auto points = std::vector<glm::vec2>{
//            {0.0f, 4.0f},
//            {1.0f, 3.0f},
//            {2.0f, 2.0f},
//            {3.0f, 1.0f},
//            {4.0f, 0.0f}
//    };
//
//    for (const auto &p : points) {
//        auto q = symtensor::QuadrupoleMoment2f::FromPosition(p);
//        std::cout << p.x << " " << p.y << std::endl;
//        std::cout << q << std::endl;
//    }
//
//}
#include <emscripten/bind.h>
#include <vector>

// todo

std::vector<double> optimize(double param1, double param2) {
    std::vector<double> result;
    // Perform some complex optimization
    for (int i = 0; i < 10; ++i) {
        result.push_back(param1 * param2 * i);  // Example calculation
    }
    return result;
}

EMSCRIPTEN_BINDINGS(my_module) {
        emscripten::function("optimize", &optimize);
        emscripten::register_vector<double>("VectorDouble");
}