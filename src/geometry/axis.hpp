#ifndef CPP_ENGINE_AXIS_HPP
#define CPP_ENGINE_AXIS_HPP

#include <concepts>

namespace engine::axis {

struct Axis_Base {};

template <class T>
concept Axis = std::derived_from<T, Axis_Base>;

struct X_t: Axis_Base {};
struct Y_t: Axis_Base {};
struct Z_t: Axis_Base {};

inline constexpr auto X = X_t{};
inline constexpr auto Y = Y_t{};
inline constexpr auto Z = Z_t{};

} // namespace engine::axis

#endif //CPP_ENGINE_AXIS_HPP
