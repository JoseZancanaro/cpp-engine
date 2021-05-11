#ifndef CPP_ENGINE_ACCESSORS_HPP
#define CPP_ENGINE_ACCESSORS_HPP

#include <type_traits>
#include <array>

template <class Container, class Data_Accessor = decltype([]<class Obj>(Obj obj) -> auto& { return obj->container; })>
struct Accessors_For {

private:
    inline constexpr auto get_container() noexcept -> auto& {
        return Data_Accessor{}(static_cast<std::remove_const_t<Container>*>(this));
    }

    inline constexpr auto get_container() const noexcept -> auto const& {
        return Data_Accessor{}(static_cast<std::add_const_t<Container>*>(this));
    }

public:
    [[nodiscard]] inline constexpr auto data() noexcept {
        return std::data(get_container());
    }

    [[nodiscard]] inline constexpr auto size() const noexcept {
        return std::size(get_container());
    }

    inline constexpr auto operator[](std::size_t index) -> auto& {
        return get_container()[index];
    }

    inline constexpr auto operator[](std::size_t index) const -> auto const& {
        return get_container()[index];
    }

    inline constexpr auto begin() noexcept {
        return std::begin(get_container());
    }

    inline constexpr auto end() noexcept {
        return std::end(get_container());
    }

    [[nodiscard]] inline constexpr auto cbegin() const noexcept {
        return std::cbegin(get_container());
    }

    [[nodiscard]] inline constexpr auto cend() const noexcept {
        return std::cend(get_container());
    }

    inline constexpr auto rbegin() noexcept {
        return std::rbegin(get_container());
    }

    inline constexpr auto rend() noexcept {
        return std::rend(get_container());
    }

    [[nodiscard]] inline constexpr auto crbegin() const noexcept {
        return std::crbegin(get_container());
    }

    [[nodiscard]] inline constexpr auto crend() const noexcept {
        return std::crend(get_container());
    }
};

#endif //CPP_ENGINE_ACCESSORS_HPP
