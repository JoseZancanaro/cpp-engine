#ifndef CPP_ENGINE_NUMERIC_HPP
#define CPP_ENGINE_NUMERIC_HPP

#include <concepts>

namespace engine {

template <class T>
concept Number = std::integral<std::remove_reference_t<T>> || std::floating_point<std::remove_reference_t<T>>;

}

#endif //CPP_ENGINE_NUMERIC_HPP
