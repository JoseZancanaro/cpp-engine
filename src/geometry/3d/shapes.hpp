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

    auto norm() -> void {
        auto bb_x = std::ranges::minmax_element(vertex, less(axis::X));
        auto bb_y = std::ranges::minmax_element(vertex, less(axis::Y));
        auto bb_z = std::ranges::minmax_element(vertex, less(axis::Z));

        auto max_x = bb_x.max->x, min_x = bb_x.min->x;
        auto max_y = bb_y.max->y, min_y = bb_y.min->y;
        auto max_z = bb_z.max->z, min_z = bb_z.min->z;

        for (auto & v : vertex) {
            v.x = (v.x - min_x) / (max_x - min_x);
            v.y = (v.y - min_y) / (max_y - min_y);
            v.z = (v.z - min_z) / (max_z - min_z);
        }
    }
};

} // namespace engine::space3D

#endif //CPP_ENGINE_SHAPES_HPP
