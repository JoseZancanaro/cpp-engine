cmake_minimum_required(VERSION 3.17)
project(cpp-engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "/Od")

include(${CMAKE_CURRENT_SOURCE_DIR}/build/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(cpp-engine
        src/main.cpp
        src/draw.hpp
        src/pixel-buffer.hpp
        #[[ Utility ]]
        src/utility/accessors.hpp
        src/utility/concepts.hpp
        src/utility/result.hpp
        #[[ Geometry ]]
        src/geometry/core.hpp
        src/geometry/axis.hpp
        src/geometry/transform_base.hpp
        #[[ 2D Geometry ]]
        src/geometry/2d/vector.hpp
        src/geometry/2d/transform.hpp
        src/geometry/2d/shapes.hpp
        #[[ 3D Geometry ]]
        src/geometry/3d/vector.hpp
        src/geometry/3d/transform.hpp
        src/geometry/3d/shapes.hpp
        #[[ IO ]]
        src/io/obj_reader.hpp
        #[[ Tests ]]
        src/sample/rectangle.hpp
        src/sample/tetrahedron.hpp
        src/sample/perspective_tetrahedron.hpp
        src/sample/wavefront_runner.hpp)
target_link_libraries(cpp-engine ${CONAN_LIBS})
