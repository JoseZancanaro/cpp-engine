#ifndef CPP_ENGINE_AXIS_HPP
#define CPP_ENGINE_AXIS_HPP

namespace engine::axis {

struct X_t {};
struct Y_t {};
struct Z_t {};

inline constexpr auto X = X_t{};
inline constexpr auto Y = Y_t{};
inline constexpr auto Z = Z_t{};

} // namespae engine::axis

#endif //CPP_ENGINE_AXIS_HPP
