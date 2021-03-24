#ifndef CPP_ENGINE_PIXEL_BUFFER_HPP
#define CPP_ENGINE_PIXEL_BUFFER_HPP

#include <algorithm>
#include <vector>
#include <array>

namespace engine {

auto flat_index(std::size_t x, std::size_t y, std::size_t width, std::size_t length = 4u) -> std::size_t {
    return (y * width + x) * length;
}

template <typename Pixel_Type, std::size_t Length>
struct Buffer_2D {
    std::vector<Pixel_Type> elements;
    std::size_t width;
    std::size_t height;

    Buffer_2D(std::size_t width, std::size_t height, Pixel_Type value = {})
        : width(width), height(height), elements(std::vector<Pixel_Type>(width * height * Length, value))
    {}

    auto set(std::size_t x, std::size_t y, Pixel_Type value) -> void {
        auto index = flat_index(x, y, width, Length);
        if (index >= 0 && index < std::size(elements)) { // @TODO remove check
            elements.at() = value;
        }
    }

    auto set(std::size_t x, std::size_t y, std::array<Pixel_Type, Length> value) -> void {
        auto index = flat_index(x, y, width, Length);
        if (index >= 0 && index + std::size(value) < std::size(elements)) { // @TODO remove check
            std::copy_n(std::begin(value), std::size(value),
                        std::next(std::begin(elements), index)); // data.begin() + flat_index(...)
        }
    }

    auto size() noexcept -> typename std::vector<Pixel_Type>::size_type {
        return std::size(elements);
    }

    auto data() noexcept -> typename std::vector<Pixel_Type>::pointer {
        return elements.data();
    }

    auto begin() noexcept -> typename std::vector<Pixel_Type>::iterator {
        return std::begin(elements);
    }

    auto end() noexcept -> typename std::vector<Pixel_Type>::iterator {
        return std::end(elements);
    }

    auto rbegin() noexcept -> typename std::vector<Pixel_Type>::iterator {
        return std::rbegin(elements);
    }

    auto rend() noexcept -> typename std::vector<Pixel_Type>::iterator {
        return std::rend(elements);
    }

    auto cbegin() const noexcept -> typename std::vector<Pixel_Type>::const_iterator {
        return std::cbegin(elements);
    }

    auto cend() const noexcept -> typename std::vector<Pixel_Type>::const_iterator {
        return std::cend(elements);
    }

    auto crbegin() const noexcept -> typename std::vector<Pixel_Type>::const_iterator {
        return std::crbegin(elements);
    }

    auto crend() const noexcept -> typename std::vector<Pixel_Type>::const_iterator {
        return std::crend(elements);
    }
};

using Basic_RGBA_Buffer = Buffer_2D<std::uint8_t, 4>;

}

#endif //CPP_ENGINE_PIXEL_BUFFER_HPP
