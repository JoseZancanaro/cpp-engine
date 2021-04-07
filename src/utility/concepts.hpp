#ifndef CPP_ENGINE_CONCEPTS_HPP
#define CPP_ENGINE_CONCEPTS_HPP

#include <concepts>
#include <ranges>

namespace engine {

template <class T>
concept Number = std::integral<std::remove_reference_t<T>> || std::floating_point<std::remove_reference_t<T>>;

}

#endif //CPP_ENGINE_CONCEPTS_HPP
