#ifndef CPP_ENGINE_2D_SHAPES_HPP
#define CPP_ENGINE_2D_SHAPES_HPP

namespace engine {

template <typename Shape>
auto centroid(Shape && shape) {
    return shape.centroid();
}

template <typename T>
struct Rectangle {
    std::array<Vector_2D<T>, 4> vertex;

    auto centroid() -> Vector_2D<T> {
        auto const& [p1, p2, p3, p4] = vertex;
        return { p1.x + ((p3.x - p1.x) / 2), p1.y + ((p3.y - p1.y) / 2) };
    }
};

template <typename T, typename R = T>
struct Circle {
    Vector_2D<T> center;
    R radius;

    auto centroid() -> Vector_2D<T> {
        return center;
    }
};

template <typename T>
struct Polygon {
    std::vector<Vector_2D<T>> vertex;
};

}

#endif //CPP_ENGINE_2D_SHAPES_HPP
