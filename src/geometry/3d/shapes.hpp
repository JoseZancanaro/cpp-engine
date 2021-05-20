#ifndef CPP_ENGINE_SHAPES_HPP
#define CPP_ENGINE_SHAPES_HPP

#include <fmt/core.h>

#include <array>
#include <vector>
#include <numeric>
#include <ranges>

#include "vector.hpp"

namespace engine::space3D {

template <class T>
struct Tetrahedron {
    std::array<Vector_3D<T>, 4> vertex;
};

template <class T>
struct Solid {
    struct Face_Indexer {
        std::vector<std::size_t> indexes;
    };

    std::vector<Vector_3D<T>> vertex;
    std::vector<Face_Indexer> faces;
};

} // namespace engine::space3D

#endif //CPP_ENGINE_SHAPES_HPP
