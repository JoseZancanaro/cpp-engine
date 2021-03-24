#ifndef CPP_ENGINE_CORE_HPP
#define CPP_ENGINE_CORE_HPP

#include <algorithm>
#include <vector>
#include <array>
#include <cmath>

#include "../pixel-buffer.hpp"

namespace engine {

template <typename T>
struct Vector_2D {
    T x;
    T y;
};

/* Shorthands */
using Vector_2Di = Vector_2D<int>;

struct Mat3 {
    using mat3_t = std::array<std::array<double, 3>, 3>;

    mat3_t data;

    static auto identity() -> Mat3 {
        return {{{
            { 1, 0, 0 },
            { 0, 1, 0 },
            { 0, 0, 1 }
        }}};
    }

    static auto translate(double a, double b) -> Mat3 {
        return {{{
            { 1, 0, a },
            { 0, 1, b },
            { 0, 0, 1 }
        }}};
    }

    static auto rotate(double angle) -> Mat3 {
        return {{{
             {std::cos(angle), std::sin(angle), 0},
             {-std::sin(angle), std::cos(angle), 0},
             {0, 0, 1}
        }}};
    }

    constexpr auto operator[](std::size_t index) -> decltype(std::declval<mat3_t &>()[mat3_t::size_type{}]){
        return data[index];
    }

    constexpr auto operator[](std::size_t index) const -> decltype(std::declval<mat3_t const&>()[mat3_t::size_type{}]){
        return data[index];
    }

    constexpr auto size() const -> mat3_t::size_type {
        return std::size(data);
    }

    constexpr auto begin() -> mat3_t::iterator {
        return std::begin(data);
    }

    constexpr auto end() -> mat3_t::iterator {
        return std::end(data);
    }
};

auto operator*(Mat3 const& left, Mat3 const& right) -> Mat3 {
    auto product = Mat3{};

    for (auto i = 0ul; i < std::size(product); ++i) {
        for (auto j = 0ul; j < std::size(product); ++j) {
            product[i][j] = left[i][j] * right[i][j];
        }
    }

    return product;
}

template <typename T>
auto apply_transform(Vector_2D<T> vec, Mat3 transform) -> Vector_2D<T> {
    return {
        static_cast<T>(vec.x * transform[0][0] + vec.y * transform[0][1] + /*vec.w * */ transform[0][2]),
        static_cast<T>(vec.x * transform[1][0] + vec.y * transform[1][1] + /*vec.w * */ transform[1][2])
    };
}

template <typename T>
auto operator*(Vector_2D<T> vec, Mat3 transform) -> Vector_2D<T> {
    return apply_transform(vec, transform);
}

template <typename T, typename Scalar>
auto translate(Vector_2D<T> vec, Scalar a, Scalar b) -> Vector_2D<T> {
    return { vec.x + a, vec.y + b };
}

template <typename T, typename Scalar>
auto scale(Vector_2D<T> vec, Scalar a, Scalar b) -> Vector_2D<T> {
    return { vec.x * a, vec.y * b };
}

template <typename T, typename Scalar>
auto rotate(Vector_2D<T> vec, Scalar theta) -> Vector_2D<T> {
    return { vec.x * std::cos(theta) + vec.y * std::sin(theta),
             - vec.x * std::sin(theta) + vec.y * std::cos(theta) };
}

template <typename T, typename Scalar>
auto shear(Vector_2D<T> vec, Scalar a, Scalar b) -> Vector_2D<T> {
    return { vec.x + a * vec.y, vec.y + b * vec.x };
}

}

#endif //CPP_ENGINE_CORE_HPP
