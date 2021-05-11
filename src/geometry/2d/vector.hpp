#ifndef CPP_ENGINE_2D_PRIMITIVES_HPP
#define CPP_ENGINE_2D_PRIMITIVES_HPP

#include "../../utility/concepts.hpp"

namespace engine::space2D {

template <class V>
concept Dim2_Vec = requires(std::remove_cvref_t<V> v) {
    { v.x } -> Number;
    { v.y } -> Number;
};

template <Number T>
struct Vector_2D {
    T x;
    T y;
};

/* Shorthands */
using Vector_2Di = Vector_2D<int>;
using Vector_2Df = Vector_2D<float>;
using Vector_2Dd = Vector_2D<double>;

} // namespace engine::space2D

#endif //CPP_ENGINE_PRIMITIVES_HPP
