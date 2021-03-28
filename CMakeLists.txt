cmake_minimum_required(VERSION 3.17)
project(cpp-engine)

set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(cpp-engine src/main.cpp src/geometry/core.hpp src/pixel-buffer.hpp src/geometry/primitives.hpp src/draw.hpp src/geometry/2d/vector.hpp src/geometry/2d/transform.hpp src/geometry/2d/shapes.hpp)
target_link_libraries(cpp-engine ${CONAN_LIBS})
