#ifndef CPP_ENGINE_2D_PRIMITIVES_HPP
#define CPP_ENGINE_2D_PRIMITIVES_HPP

namespace engine {

template <typename T>
struct Vector_2D {
    T x;
    T y;
};

/* Shorthands */
using Vector_2Di = Vector_2D<int>;
using Vector_2Df = Vector_2D<float>;
using Vector_2Dd = Vector_2D<double>;

}

#endif //CPP_ENGINE_PRIMITIVES_HPP
