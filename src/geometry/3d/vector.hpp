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

template <Number T>
auto magnitude(Vector_3D<T> const& vector) -> T {
    return T(std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z));
}

template <Number T>
auto normalize(Vector_3D<T> const& vector) -> Vector_3D<T> {
    auto mag = 1 / magnitude(vector);

    if (mag != T(0)) {
        return { vector.x * mag, vector.y * mag, vector.z * mag };
    }

    return vector;
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
