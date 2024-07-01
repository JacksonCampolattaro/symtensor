


add_executable(gravity2d
        bindings.cpp
)
set_target_properties(gravity2d PROPERTIES
#        OUTPUT_NAME "optimize"
        SUFFIX ".html"
        LINK_FLAGS "--bind -o bindings.js"
)
target_link_libraries(
        gravity2d PRIVATE
        symtensor::symtensor
)
