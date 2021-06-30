cmake_minimum_required(VERSION 3.17)
project(cpp-engine)

set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(cpp-engine
        src/main.cpp
        src/gl.hpp
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
        src/scene/sample/glwavefront_runner.hpp
        src/scene/Buffered_Entity_Base.hpp
        src/scene/Model.hpp
        src/scene/Entity_Owner.hpp
        src/scene/md2/header.hpp
        src/scene/md2/loader.hpp
        src/scene/md2/Model.hpp
        src/scene/shader/core.hpp
        src/gl-shaders/basic_vs.hpp
        src/gl-shaders/basic_fs.hpp
        src/texture/core.hpp
        src/model/Vbo_Grid.hpp
        src/gl-shaders/grid_vs.hpp
        src/gl-shaders/light_fs.hpp src/model/Simple_Quad.hpp src/gl-shaders/light_vs.hpp)

target_link_libraries(cpp-engine ${CONAN_LIBS})

# OpenGL
#set(OpenGL opengl32)
#target_link_libraries(cpp-engine ${OpenGL})