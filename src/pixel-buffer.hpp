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
        : width(width), height(height), elements(std::vector<Pixel_Type>(width * height * Length))
    {}

    auto set(std::size_t x, std::size_t y, Pixel_Type value) {
        elements.at(flat_index(x, y, width, Length)) = value;
    }

    auto set(std::size_t x, std::size_t y, std::array<Pixel_Type, Length> value) {
        std::copy_n(std::begin(value), std::size(value),
                        std::next(std::begin(elements), flat_index(x, y, width, Length))); // data.begin() + flat_index(...)
    }
};

using Basic_RGBA_Buffer = Buffer_2D<std::uint8_t, 4>;

}

#endif //CPP_ENGINE_PIXEL_BUFFER_HPP
