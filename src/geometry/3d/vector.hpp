#ifndef CPP_ENGINE_VECTOR_HPP
#define CPP_ENGINE_VECTOR_HPP

#include "numeric.hpp"

namespace engine::space3D {

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

} // namespace engine::space3D

#endif //CPP_ENGINE_VECTOR_HPP
