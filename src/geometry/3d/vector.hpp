#ifndef CPP_ENGINE_VECTOR_HPP
#define CPP_ENGINE_VECTOR_HPP

#include <cmath>
#include <type_traits>

#include "../axis.hpp"
#include "../../utility/concepts.hpp"

namespace engine::space3D {

template <class V>
concept Dim3_Vec = requires(std::remove_cvref_t<V> v) {
    { v.x } -> Number;
    { v.y } -> Number;
    { v.z } -> Number;
};

template <Number T>
struct Vector_3D {
    T x;
    T y;
    T z;
};

/* Shorthands */
using Vector_3Di = Vector_3D<int>;
using Vector_3Df = Vector_3D<float>;
using Vector_3Dd = Vector_3D<double>;

/* Utility */
template <Dim3_Vec U, Dim3_Vec V>
auto dot_product(U const& u, V const& v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

template <Dim3_Vec U>
auto magnitude(U const& u) {
    return std::sqrt(dot_product(u, u));
}

template <Dim3_Vec U>
auto normalize(U const& u) -> U {
    if (auto mag = magnitude(u); mag > 0) {
        auto inv = 1 / mag;
        return { u.x * inv, u.y * inv, u.z * inv };
    }

    return u;
}

template <Dim3_Vec U, Dim3_Vec V>
auto minus(U const& u, V const& v) -> U {
    return U{ u.x - v.x, u.y - v.y, u.z - v.z };
}

template <Dim3_Vec U, Dim3_Vec V>
auto operator-(U const& u, V const& v) -> U {
    return minus(u, v);
}

template <Dim3_Vec U, Dim3_Vec V>
auto plus(U const& u, V const& v) -> U {
    return U{ u.x + v.x, u.y + v.y, u.z + v.z };
}

template <Dim3_Vec U, Dim3_Vec V>
auto operator+(U const& u, V const& v) -> U {
    return plus(u, v);
}

template <Dim3_Vec U, Dim3_Vec V>
auto distance_squared(U const& u, V const& v) {
    auto delta = u - v;
    return dot_product(delta, delta);
}

template <axis::Axis A>
struct less_t {};

template <Dim3_Vec U, Dim3_Vec V>
inline constexpr auto less(axis::X_t, U && u, V && v) -> bool {
    return u.x < v.x;
}

template <Dim3_Vec U, Dim3_Vec V>
inline constexpr auto less(axis::Y_t, U && u, V && v) -> bool {
    return u.y < v.y;
}

template <Dim3_Vec U, Dim3_Vec V>
inline constexpr auto less(axis::Z_t, U && u, V && v) -> bool {
    return u.z < v.z;
}

template <>
struct less_t<axis::X_t> {
    template <Dim3_Vec U, Dim3_Vec V>
    inline constexpr auto operator()(U && u, V && v) -> bool {
        return less(axis::X, u, v);
    }
};

inline constexpr auto less(axis::X_t) -> less_t<axis::X_t> {
    return {};
}

template <>
struct less_t<axis::Y_t> {
    template <Dim3_Vec U, Dim3_Vec V>
    inline constexpr auto operator()(U && u, V && v) -> bool {
        return less(axis::Y, u, v);
    }
};

inline constexpr auto less(axis::Y_t) -> less_t<axis::Y_t> {
    return {};
}

template <>
struct less_t<axis::Z_t> {
    template <Dim3_Vec U, Dim3_Vec V>
    inline constexpr auto operator()(U && u, V && v) -> bool {
        return less(axis::Z, u, v);
    }
};

inline constexpr auto less(axis::Z_t) -> less_t<axis::Z_t> {
    return {};
}

} // namespace engine::space3D

#endif //CPP_ENGINE_VECTOR_HPP
