find_package(Doxygen REQUIRED)
include(FetchContent)
FetchContent_Declare(
        mcss
        GIT_REPOSITORY https://github.com/mosra/m.css.git
        GIT_TAG master
)
FetchContent_MakeAvailable(mcss)
set(MCSS_DOXYGEN_EXECUTABLE ${mcss_SOURCE_DIR}/documentation/doxygen.py)

# Preprocessing doxyfile template
set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/../doc/Doxyfile.in)
set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
set(MCSS_IN ${CMAKE_CURRENT_SOURCE_DIR}/../doc/conf.py)
set(MCSS_OUT ${CMAKE_CURRENT_BINARY_DIR}/conf.py)
configure_file(${MCSS_IN} ${MCSS_OUT} @ONLY)
set(DOXYGEN_MCSS_IN ${CMAKE_CURRENT_SOURCE_DIR}/../doc/Doxyfile-mcss.in)
set(DOXYGEN_MCSS_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile-mcss)
configure_file(${DOXYGEN_MCSS_IN} ${DOXYGEN_MCSS_OUT} @ONLY)
set(STYLE_IN ${CMAKE_CURRENT_SOURCE_DIR}/../doc/style.css)
set(STYLE_OUT ${CMAKE_CURRENT_BINARY_DIR}/style.css)
configure_file(${STYLE_IN} ${STYLE_OUT} @ONLY)

add_custom_target(
        documentation
        COMMAND ${MCSS_DOXYGEN_EXECUTABLE} ${MCSS_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen-mcss"
        DEPENDS ${MCSS_OUT} ${DOXYGEN_OUT} ${DOXYGEN_MCSS_OUT}
        VERBATIM
)