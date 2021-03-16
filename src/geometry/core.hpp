#ifndef CPP_ENGINE_CORE_HPP
#define CPP_ENGINE_CORE_HPP

#include <algorithm>
#include <vector>
#include <array>

#include "../pixel-buffer.hpp"

namespace engine {

template <typename T>
struct Vector_2D {
    T x;
    T y;
};

using Vector_2Di = Vector_2D<int>;

/* ZINGL (2012) */
auto bresenham_plot(Basic_RGBA_Buffer & buffer, Vector_2Di origin, Vector_2Di target,
                         std::array<uint8_t, 4> color = { 255, 255, 255, 255 }) -> void {
    auto [x1, y1] = origin;
    auto [x2, y2] = target;

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

auto bresenham_prof(Basic_RGBA_Buffer & buffer, Vector_2Di origin, Vector_2Di target,
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
    auto slope = 1;

    if (dy < 0){
        slope = -1;
        dy = -dy;
    }

    auto y = y1;
    for (auto x = x1; x <= x2; x++){
        buffer.set(x, y, color);
        if (d <= 0){
            d += i;
        } else{
            d += j;
            y += slope;
        }
    }
}

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

}

#endif //CPP_ENGINE_CORE_HPP
