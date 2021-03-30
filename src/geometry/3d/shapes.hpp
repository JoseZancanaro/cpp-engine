#ifndef CPP_ENGINE_SHAPES_HPP
#define CPP_ENGINE_SHAPES_HPP

#include <array>

#include "vector.hpp"

namespace engine::space3D {

template <class T>
struct Tetrahedron {
    std::array<Vector_3D<T>, 4> vertex;
};

} // namespace engine::space3D

#endif //CPP_ENGINE_SHAPES_HPP
