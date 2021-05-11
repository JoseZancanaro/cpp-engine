cmake_minimum_required(VERSION 3.17)
project(cpp-engine)

set(CMAKE_CXX_STANDARD 20)

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
        #[[ RNG ]]
        src/rng/core.hpp
        #[[ Scene ]]
        src/scene/Scene.hpp
        src/scene/Entity_Base.hpp
        src/scene/Solid_Sphere.hpp
        #[[ Tests ]]
        src/scene/sample/rectangle.hpp
        src/scene/sample/tetrahedron.hpp
        src/scene/sample/perspective_tetrahedron.hpp
        src/scene/sample/wavefront_runner.hpp
        src/scene/sample/glwavefront_runner.hpp)

target_link_libraries(cpp-engine ${CONAN_LIBS})

# OpenGL
set(OpenGL opengl32)
target_link_libraries(cpp-engine ${OpenGL})