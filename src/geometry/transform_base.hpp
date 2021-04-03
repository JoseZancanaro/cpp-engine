#ifndef CPP_ENGINE_TRANSFORM_BASE_HPP
#define CPP_ENGINE_TRANSFORM_BASE_HPP

#include "../utility/concepts.hpp"

namespace engine {

template <Matrix T>
auto operator*(T const& left, T const& right) -> T {
    auto product = T{};

    for (auto i = 0u; i < std::size(product); ++i) {
        for (auto j = 0u; j < std::size(product); ++j) {
            for (auto k = 0u; k < std::size(product); ++k) {
                product[i][j] += left[i][k] * right[k][j];
            }
        }
    }

    return product;
}

}

#endif //CPP_ENGINE_TRANSFORM_BASE_HPP
