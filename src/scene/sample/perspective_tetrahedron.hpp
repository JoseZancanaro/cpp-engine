#ifndef CPP_ENGINE_PERSPECTIVE_TETRAHEDRON_HPP
#define CPP_ENGINE_PERSPECTIVE_TETRAHEDRON_HPP

#include <numbers>
#include <array>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <SFML/Graphics.hpp>

#include "../../geometry/core.hpp"
#include "../../pixel-buffer.hpp"
#include "../../draw.hpp"

template <std::size_t width, std::size_t height, class T = double>
class Perspective_Tetrahedron_Runner {
    engine::Basic_RGBA_Buffer pixels;

    sf::Texture pixels_texture;
    sf::Sprite pixels_sprite;

    /* Example data */
    engine::Tetrahedron<T> tetrahedron;
    T angle;
    T angle_step;
    T cp;
    T cp_step;

public:

    Perspective_Tetrahedron_Runner() :
        pixels(width, height, 255u),
        pixels_texture{},
        pixels_sprite{},
        tetrahedron{{ {{300, 200, 0}, {400, 300, 0}, {200, 300, 0}, {300, 250, 100}} }},
        angle{0},
        angle_step{15},
        cp{500},
        cp_step{50}
    {
        pixels_texture.create(width, height);
        pixels_texture.update(std::data(pixels));
        pixels_sprite.setTexture(pixels_texture);

        update_pixels(tetrahedron);
    }

    template <class W, class E>
    auto event_hook(W const& window, E event) -> void {
        if (event.type == sf::Event::KeyPressed) {
            /* Rotate */
            if (event.key.code == sf::Keyboard::X) {
                angle += angle_step * (1 - 2 * event.key.shift);
            }

            /* Update perspective */
            if (event.key.code == sf::Keyboard::Add) {
                cp += cp_step;
            } else if (event.key.code == sf::Keyboard::Subtract && (cp - cp_step) > 0) {
                cp -= cp_step;
            }

            auto transform = (
                    /* projection */
                    engine::Mat4::simple_perspective(cp) *
                    /* perspective */
                    engine::Mat4::translate(-50, -50, 100) *
                    /* move forth */
                    engine::Mat4::translate(0, 250, 0) *
                    /* rotate angle */
                    engine::Mat4::rotate(axis::X, angle * (std::numbers::pi / 180.0)) *
                    /* move to origin */
                    engine::Mat4::translate(0, -250, 0)
            );

            auto tetrahedron_copy = tetrahedron;
            for (auto & v : tetrahedron_copy.vertex) {
                v = v * transform;
            }

            update_pixels(tetrahedron_copy);
        }
    }

    template <class W>
    auto render(W & window) -> void {
        window.draw(pixels_sprite);
    }

private:

    template <class Shape>
    auto update_pixels(Shape shape) -> void {
        std::fill(std::begin(pixels), std::end(pixels), 255u);

        auto color = std::array<std::uint8_t, 4>{ 0, 0, 0, 255 };
        auto [p1, p2, p3, p4] = shape.vertex;

        engine::draw_triangle(pixels, engine::Triangle<T>{{ {{p1.x, p1.y}, {p2.x, p2.y}, {p3.x, p3.y}} }}, color);
        engine::draw_triangle(pixels, engine::Triangle<T>{{ {{p1.x, p1.y}, {p2.x, p2.y}, {p4.x, p4.y}} }}, color);
        engine::draw_triangle(pixels, engine::Triangle<T>{{ {{p2.x, p2.y}, {p3.x, p3.y}, {p4.x, p4.y}} }}, color);
        engine::draw_triangle(pixels, engine::Triangle<T>{{ {{p3.x, p3.y}, {p1.x, p1.y}, {p4.x, p4.y}} }}, color);

        pixels_texture.update(std::data(pixels));
    }
};

#endif //CPP_ENGINE_PERSPECTIVE_TETRAHEDRON_HPP
