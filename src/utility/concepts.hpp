#ifndef CPP_ENGINE_CONCEPTS_HPP
#define CPP_ENGINE_CONCEPTS_HPP

#include <ranges>
#include <concepts>

namespace engine {

template <class T>
concept Number = std::integral<std::remove_reference_t<T>> || std::floating_point<std::remove_reference_t<T>>;

template <class T>
concept Matrix = std::ranges::range<T>;

}

#endif //CPP_ENGINE_CONCEPTS_HPP
