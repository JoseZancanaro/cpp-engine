#ifndef CPP_ENGINE_TRANSFORM_HPP
#define CPP_ENGINE_TRANSFORM_HPP

#include <cmath>
#include <numbers>

#include "../axis.hpp"
#include "../transform_base.hpp"
#include "../../utility/accessors.hpp"

namespace engine::space3D {

using engine::operator*; /* so ADL catches from Mat4 usage */

struct Mat4 : Accessors_For<Mat4> {
    using mat4_t = std::array<std::array<double, 4>, 4>;

    mat4_t container;

    static inline double const default_fov = 1 / std::tan(90 / 2.0 * std::numbers::pi / 180);

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

    static auto scale(double dx, double dy, double dz) -> Mat4 {
        return /*Mat4*/ { .container = {{
            { dx, 0,  0,  0 },
            { 0,  dy, 0,  0 },
            { 0,  0,  dz, 0 },
            { 0,  0,  0,  1 },
        }}};
    }

    static auto rotate(axis::X_t, double angle) -> Mat4 {
        return /*Mat4*/ { .container = {{
            { 1,                0,               0,  0 },
            { 0,  std::cos(angle),  std::sin(angle), 0 },
            { 0, -std::sin(angle), std::cos(angle),  0 },
            { 0,                0,               0,  1 }
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

    static auto simple_perspective(double cp) -> Mat4 {
        return /*Mat4*/{ .container = {{
           { 1, 0, 0,      0 },
           { 0, 1, 0,      0 },
           { 0, 0, 0,      0 },
           { 0, 0, 1 / cp, 1 },
        }}};
    }

    static auto simple_perspective(double n, double q, double a = 1, double f = default_fov) -> Mat4 {
        return /*Mat4*/{ .container = {{
            { a * f, 0, 0,           0 },
            { 0,     f, 0,           0 },
            { 0,     0, q,           1 },
            { 0,     0, -(n * q), 0 },
        }}};
    }
};

template <class T>
auto apply_transform(Vector_3D<T> const& vec, Mat4 const& transform) -> Vector_3D<T> {
    auto out = Vector_3D <T> {
        T(vec.x * transform[0][0] + vec.y * transform[0][1] + vec.z * transform[0][2] + /*vec.w **/ transform[0][3]),
        T(vec.x * transform[1][0] + vec.y * transform[1][1] + vec.z * transform[1][2] + /*vec.w **/ transform[1][3]),
        T(vec.x * transform[2][0] + vec.y * transform[2][1] + vec.z * transform[2][2] + /*vec.w **/ transform[2][3])
    };

    if (auto w = T(vec.x * transform[3][0] + vec.y * transform[3][1] + vec.z * transform[3][2] + /*vec.w **/ transform[3][3]);
        w != T(0)) {
        out.x /= w;
        out.y /= w;
        out.z /= w;
    }

    return out;
}

template <class T>
auto operator*(Vector_3D<T> const& vec, Mat4 const& transform) -> Vector_3D<T> {
    return apply_transform(vec, transform);
}

} // namespace engine::space3D

#endif //CPP_ENGINE_TRANSFORM_HPP
