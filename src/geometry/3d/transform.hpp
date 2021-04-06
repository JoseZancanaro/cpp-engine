#ifndef CPP_ENGINE_TRANSFORM_HPP
#define CPP_ENGINE_TRANSFORM_HPP

#include "../axis.hpp"
#include "../../utility/accessors.hpp"

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
            { 1, 0, 0      , 0 },
            { 0, 1, 0      , 0 },
            { 0, 0, 0      , 0 },
            { 0, 0, 1 / cp , 1 },
        }}};
    }
};

auto operator*(Mat4 const& left, Mat4 const& right) -> Mat4 {
    auto product = Mat4{};

    for (auto i = 0u; i < std::size(product); ++i) {
        for (auto j = 0u; j < std::size(product); ++j) {
            for (auto k = 0u; k < std::size(product); ++k) {
                product[i][j] += left[i][k] * right[k][j];
            }
        }
    }

    return product;
}

template <class T>
auto apply_transform(Vector_3D<T> const& vec, Mat4 const& transform) -> Vector_3D<T> {
    auto vector = Vector_3D<T> {
        static_cast<T>(vec.x * transform[0][0] + vec.y * transform[0][1] + vec.z * transform[0][2] + /*vec.w **/ transform[0][3]),
        static_cast<T>(vec.x * transform[1][0] + vec.y * transform[1][1] + vec.z * transform[1][2] + /*vec.w **/ transform[1][3]),
        static_cast<T>(vec.x * transform[2][0] + vec.y * transform[2][1] + vec.z * transform[2][2] + /*vec.w **/ transform[2][3])
    };

    auto w = static_cast<T>(vec.x * transform[3][0] + vec.y * transform[3][1] + vec.z * transform[3][2] + /*vec.w **/ transform[3][3]);

    if (w != T{1}) {
        vector.x /= w;
        vector.y /= w;
        vector.z /= w;
    }

    return vector;
}

template <class T>
auto operator*(Vector_3D<T> const& vec, Mat4 const& transform) -> Vector_3D<T> {
    return apply_transform(vec, transform);
}

} // namespace engine::space3D

#endif //CPP_ENGINE_TRANSFORM_HPP
