#ifndef CPP_ENGINE_DRAW_HPP
#define CPP_ENGINE_DRAW_HPP

#include <cmath>

#include "pixel-buffer.hpp"
#include "geometry/2d/vector.hpp"

namespace engine {

/* ZINGL (2012) */
template <class P = std::uint8_t, std::size_t C = 4, class T>
auto discrete_line_plot(Buffer_2D<P, C> & buffer, T x0, T y0, T xn, T yn, std::array<P, C> const& color) -> void {
    auto x1 = std::lround(x0), y1 = std::lround(y0);
    auto x2 = std::lround(xn), y2 = std::lround(yn);

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

template <class P = std::uint8_t, std::size_t C = 4, class T>
auto discrete_line_plot(Buffer_2D<P, C> & buffer, Vector_2D<T> origin, Vector_2D<T> target,
                        std::array<P, C> const& color) -> void {
    discrete_line_plot(buffer, origin.x, origin.y, target.x, target.y, color);
}

template <class P = std::uint8_t, std::size_t C = 4, class T>
[[deprecated]]
auto bresenham(Buffer_2D<P, C> & buffer, Vector_2D<T> origin, Vector_2D<T> target,
                std::array<P, C> const& color) -> void {
    if (origin.x > target.x) {
        std::swap(origin, target);
    }

    auto x1 = std::lround(origin.x), y1 = std::lround(origin.y);
    auto x2 = std::lround(target.x), y2 = std::lround(target.y);

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

} // namespace engine

namespace engine::details {

template <class P = std::uint8_t, std::size_t C = 4, class T>
auto draw_solid_face(Buffer_2D<P, C> & buffer, Solid<T> solid, std::size_t face, std::array<P, C> const& color) -> void {
    auto const& [ vertex, faces ] = solid;

    if (std::size(faces[face].indexes) > 2) {
        for (auto i = 0ul; i < std::size(faces[face].indexes) - 1; ++i) {
            /* draw line from current to next coordinate */
            auto const& current = vertex[faces[face].indexes[i] - 1];
            auto const& next = vertex[faces[face].indexes[i + 1] - 1];

            discrete_line_plot(buffer, current.x, current.y, next.x, next.y, color);
        }

        /* draw line back */
        auto const& first = vertex[faces[face].indexes.front() - 1];
        auto const& last = vertex[faces[face].indexes.back() - 1];

        discrete_line_plot(buffer, last.x, last.y, first.x, first.y, color);
    }
}

} // namespace engine::details

namespace engine {

template <class P = std::uint8_t, std::size_t C = 4, class T>
auto draw_rect(Buffer_2D<P, C> & buffer, Rectangle<T> rect, std::array<P, C> const& color) -> void {
    discrete_line_plot(buffer, rect.vertex[0], rect.vertex[1], color);
    discrete_line_plot(buffer, rect.vertex[1], rect.vertex[2], color);
    discrete_line_plot(buffer, rect.vertex[2], rect.vertex[3], color);
    discrete_line_plot(buffer, rect.vertex[3], rect.vertex[0], color);
}

template <class P = std::uint8_t, std::size_t C = 4, class T>
auto draw_triangle(Buffer_2D<P, C> & buffer, Triangle<T> triangle, std::array<P, C> const& color) -> void {
    discrete_line_plot(buffer, triangle.vertex[0], triangle.vertex[1], color);
    discrete_line_plot(buffer, triangle.vertex[1], triangle.vertex[2], color);
    discrete_line_plot(buffer, triangle.vertex[2], triangle.vertex[0], color);
}

template <class P = std::uint8_t, std::size_t C = 4, class T>
auto draw_solid(Buffer_2D<P, C> & buffer, Solid<T> solid, std::array<P, C> const& color) -> void {
    for (auto face = 0ul; face < std::size(solid.faces); ++face) {
        details::draw_solid_face(buffer, solid, face, color);
    }
}

} // namespace engine

#endif //CPP_ENGINE_DRAW_HPP
