#ifndef CPP_ENGINE_2D_TRANSFORM_HPP
#define CPP_ENGINE_2D_TRANSFORM_HPP

#include "vector.hpp"
#include "../../utility/accessors.hpp"

namespace engine::space2D {

struct Mat3 : Accessors_For<Mat3> {
    using mat3_t = std::array<std::array<double, 3>, 3>;

    mat3_t container;

    static auto identity() -> Mat3 {
        return /*Mat3*/ { .container = {{
            { 1, 0, 0 },
            { 0, 1, 0 },
            { 0, 0, 1 }
        }}};
    }

    static auto translate(double dx, double dy) -> Mat3 {
        return /*Mat3*/ { .container = {{
            { 1, 0, dx },
            { 0, 1, dy },
            { 0, 0, 1  }
        }}};
    }

    static auto rotate(double angle) -> Mat3 {
        return /*Mat3*/ { .container = {{
            { std::cos(angle),  std::sin(angle), 0 },
            { -std::sin(angle), std::cos(angle), 0 },
            { 0,                0,               1 }
        }}};
    }
};

template <class T>
auto apply_transform(Vector_2D<T> vec, Mat3 transform) -> Vector_2D<T> {
    return {
        static_cast<T>(vec.x * transform[0][0] + vec.y * transform[0][1] + /*vec.w * */ transform[0][2]),
        static_cast<T>(vec.x * transform[1][0] + vec.y * transform[1][1] + /*vec.w * */ transform[1][2])
    };
}

template <class T>
auto operator*(Vector_2D<T> vec, Mat3 transform) -> Vector_2D<T> {
    return apply_transform(vec, transform);
}

/**
 * Raw Transformations
 * */

template <class T, class Scalar>
auto translate(Vector_2D<T> vec, Scalar a, Scalar b) -> Vector_2D<T> {
    return { vec.x + a, vec.y + b };
}

template <class T, class Scalar>
auto scale(Vector_2D<T> vec, Scalar a, Scalar b) -> Vector_2D<T> {
    return { vec.x * a, vec.y * b };
}

template <class T, class Scalar>
auto rotate(Vector_2D<T> vec, Scalar theta) -> Vector_2D<T> {
    return { vec.x * std::cos(theta) + vec.y * std::sin(theta),
             - vec.x * std::sin(theta) + vec.y * std::cos(theta) };
}

template <class T, class Scalar>
auto shear(Vector_2D<T> vec, Scalar a, Scalar b) -> Vector_2D<T> {
    return { vec.x + a * vec.y, vec.y + b * vec.x };
}

} /* namespace engine::space2D */

#endif //CPP_ENGINE_2D_TRANSFORM_HPP
