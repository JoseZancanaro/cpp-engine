#ifndef CPP_ENGINE_DRAW_HPP
#define CPP_ENGINE_DRAW_HPP

#include "pixel-buffer.hpp"
#include "geometry/2d/vector.hpp"

namespace engine {

/* ZINGL (2012) */
template <typename Pixel_Type = std::uint8_t, std::size_t Color_Length = 4, typename T>
auto discrete_line_plot(Buffer_2D<Pixel_Type, Color_Length> & buffer, Vector_2D<T> origin, Vector_2D<T> target,
                            std::array<Pixel_Type, Color_Length> color) -> void {
    auto x1 = static_cast<int>(origin.x), y1 = static_cast<int>(origin.y);
    auto x2 = static_cast<int>(target.x), y2 = static_cast<int>(target.y);

    auto dx = std::abs(x2 - x1);
    auto dy = -std::abs(y2 - y1);
    auto error = dx + dy;

    auto direction_x = x1 < x2 ? 1 : -1;
    auto direction_y = y1 < y2 ? 1 : -1;

    auto x = x1, y = y1;

    while (x != x2 || y != y2) {
        buffer.set(x, y, color);

        auto double_error = error * 2;

        if (double_error >= dy) {
            x += direction_x;
            error += dy;
        }

        if (double_error <= dx) {
            y += direction_y;
            error += dx;
        }
    }
}

[[deprecated]]
auto bresenham(Basic_RGBA_Buffer & buffer, Vector_2Di origin, Vector_2Di target,
               std::array<uint8_t, 4> color = { 255, 255, 255, 255 }) -> void {
    if (origin.x > target.x) {
        std::swap(origin, target);
    }

    auto [x1, y1] = origin;
    auto [x2, y2] = target;

    auto dx = x2 - x1;
    auto dy = y2 - y1;

    auto i = 2 * dy;
    auto j = 2 * (dy - dx);
    auto d = i - dx;

    auto y = y1, x_max = x2;

    for (auto x = x1; x < x_max; ++x) {
        buffer.set(x, y, color);

        if (d < 0) {
            d += i;
        } else {
            d += j;
            ++y;
        }
    }
}

template <typename Pixel_Type = std::uint8_t, std::size_t Color_Length = 4, typename T>
auto draw_rect(Buffer_2D<Pixel_Type, Color_Length> & buffer, Rectangle<T> rect, std::array<Pixel_Type, Color_Length> color) -> void {
    discrete_line_plot(buffer, rect.vertex[0], rect.vertex[1], color);
    discrete_line_plot(buffer, rect.vertex[1], rect.vertex[2], color);
    discrete_line_plot(buffer, rect.vertex[2], rect.vertex[3], color);
    discrete_line_plot(buffer, rect.vertex[3], rect.vertex[0], color);
}

}

#endif //CPP_ENGINE_DRAW_HPP
