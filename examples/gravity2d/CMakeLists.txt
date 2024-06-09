
add_executable(gravity2d
        main.cpp
)
target_link_libraries(
        gravity2d PRIVATE
        symtensor::symtensor
)
