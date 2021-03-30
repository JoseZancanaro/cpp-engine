#ifndef CPP_ENGINE_TRANSFORM_HPP
#define CPP_ENGINE_TRANSFORM_HPP

#include "../axis.hpp"
#include "../../container_wrapper.hpp"

namespace engine::space3D {

struct Mat4 : Accessors_For<Mat4> {
    using mat4_t = std::array<std::array<double, 4>, 4>;

    mat4_t container;

    static auto identity() -> Mat4 {
        return /*Mat4*/ { .container = {{
            { 1, 0, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 1 },
        }}};
    }

    static auto translate(double dx, double dy, double dz) -> Mat4 {
        return /*Mat4*/ { .container = {{
            { 1, 0, 0, dx },
            { 0, 1, 0, dy },
            { 0, 0, 1, dz },
            { 0, 0, 0, 1  },
        }}};
    }

    static auto rotate(axis::X_t, double angle) -> Mat4 {
        return /*Mat4*/ { .container = {{
            { 1,                0,               0, 0 },
            { 0, std::cos(angle),  std::sin(angle), 0 },
            { 0, -std::sin(angle), std::cos(angle), 0 },
            { 0,                0,               0, 1 }
        }}};
    }

    static auto rotate(axis::Y_t, double angle) -> Mat4 {
        return /*Mat4*/ { .container = {{
            { std::cos(angle),  0, std::sin(angle), 0 },
            { 0,                1,               0, 0 },
            { -std::sin(angle), 0, std::cos(angle), 0 },
            { 0,                0,               0, 1 }
        }}};
    }

    static auto rotate(axis::Z_t, double angle) -> Mat4 {
        return /*Mat4*/ { .container = {{
            { std::cos(angle),  std::sin(angle), 0, 0 },
            { -std::sin(angle), std::cos(angle), 0, 0 },
            { 0,                0,               1, 0 },
            { 0,                0,               0, 1 }
        }}};
    }
};

template <class T>
auto apply_transform(Vector_3D<T> vec, Mat4 transform) -> Vector_3D<T> {
    return {
        static_cast<T>(vec.x * transform[0][0] + vec.y * transform[0][1] + vec.z * transform[0][2] + /*vec.w **/ transform[0][3]),
        static_cast<T>(vec.x * transform[1][0] + vec.y * transform[1][1] + vec.z * transform[1][2] + /*vec.w **/ transform[1][3]),
        static_cast<T>(vec.x * transform[2][0] + vec.y * transform[2][1] + vec.z * transform[2][2] + /*vec.w **/ transform[2][3])
    };
}

template <class T>
auto operator*(Vector_3D<T> vec, Mat4 transform) -> Vector_3D<T> {
    return apply_transform(vec, transform);
}

} // namespace engine::space3D

#endif //CPP_ENGINE_TRANSFORM_BASE_HPP
