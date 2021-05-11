#ifndef CPP_ENGINE_RNG_CORE_HPP
#define CPP_ENGINE_RNG_CORE_HPP

#include <functional>
#include <random>

namespace engine {

template <typename T = double>
using Random_Generator = std::function<auto () -> T>;

template <typename T = double>
auto make_default_generator() -> Random_Generator<T> {
    return [rng = std::default_random_engine(std::random_device{}()),
            dist = std::uniform_real_distribution(T(0), T(1))]() mutable -> T {
        return T(dist(rng));
    };
}

}

#endif //CPP_ENGINE_CORE_HPP
