cmake_minimum_required(VERSION 3.17)
project(cpp-engine)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(cpp-engine src/main.cpp src/geometry/core.hpp src/pixel-buffer.hpp)
target_link_libraries(cpp-engine ${CONAN_LIBS})
