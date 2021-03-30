#ifndef CPP_ENGINE_PIXEL_BUFFER_HPP
#define CPP_ENGINE_PIXEL_BUFFER_HPP

#include <algorithm>
#include <vector>
#include <array>

#include "container_wrapper.hpp"

namespace engine {

auto flat_index(std::size_t x, std::size_t y, std::size_t width, std::size_t length = 4u) -> std::size_t {
    return (y * width + x) * length;
}

template <class Pixel_Type, std::size_t Length>
struct Buffer_2D : Accessors_For<Buffer_2D<Pixel_Type, Length>> {
    using Pixel_Buffer = std::vector<Pixel_Type>;

    std::vector<Pixel_Type> container;
    std::size_t width;
    std::size_t height;

    Buffer_2D(std::size_t width, std::size_t height, Pixel_Type value = {})
        : width(width), height(height), container(std::vector<Pixel_Type>(width * height * Length, value))
    {}

    auto set(std::size_t x, std::size_t y, Pixel_Type value) -> void {
        auto index = flat_index(x, y, width, Length);
        if (index >= 0 && index < std::size(container)) { // @TODO remove check
            container.at() = value;
        }
    }

    auto set(std::size_t x, std::size_t y, std::array<Pixel_Type, Length> const& value) -> void {
        auto index = flat_index(x, y, width, Length);
        if (index >= 0 && index + std::size(value) < std::size(container)) { // @TODO remove check
            std::copy_n(std::begin(value), std::size(value),
                        std::next(std::begin(container), index)); // data.begin() + flat_index(...)
        }
    }
};

using Basic_RGBA_Buffer = Buffer_2D<std::uint8_t, 4>;

}

#endif //CPP_ENGINE_PIXEL_BUFFER_HPP
