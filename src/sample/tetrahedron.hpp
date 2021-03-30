#ifndef CPP_ENGINE_TETRAHEDRON_HPP
#define CPP_ENGINE_TETRAHEDRON_HPP

#include <numbers>
#include <array>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <SFML/Graphics.hpp>

#include "../geometry/core.hpp"
#include "../pixel-buffer.hpp"
#include "../draw.hpp"

template <std::size_t width, std::size_t height, class T = double>
class Tetrahedron_Runner {
    engine::Basic_RGBA_Buffer pixels;

    sf::Texture pixels_texture;
    sf::Sprite pixels_sprite;

    /* Example data */
    engine::Tetrahedron<T> tetrahedron;
    engine::Vector_2D<T> click;
    T shift;
    T angle;

public:

    Tetrahedron_Runner() :
        pixels(width, height, 255u),
        pixels_texture{},
        pixels_sprite{},
        tetrahedron{{ {{300, 200, 0}, {400, 300, 0}, {200, 300, 0}, {300, 250, 100}} }},
        click{0, 0},
        shift{10},
        angle{30}
    {
        pixels_texture.create(width, height);
        pixels_texture.update(std::data(pixels));
        pixels_sprite.setTexture(pixels_texture);

        update_pixels();
    }

    template <class W, class E>
    auto event_hook(W const& window, E event) -> void {
        if (event.type == sf::Event::KeyPressed) {
            /* Treat rect input */
            auto transform = [&]() -> engine::Mat4 {
                /* Translate */
                if (event.key.code == sf::Keyboard::Left) {
                    return engine::Mat4::translate(-shift, 0, 0);
                } else if (event.key.code == sf::Keyboard::Right) {
                    return engine::Mat4::translate(shift, 0, 0);
                } else if (event.key.code == sf::Keyboard::Up) {
                    return engine::Mat4::translate(0, -shift, 0);
                } else if (event.key.code == sf::Keyboard::Down) {
                    return engine::Mat4::translate(0, shift, 0);
                }

                /* Rotate */
                if (event.key.code == sf::Keyboard::X) {
                    return engine::Mat4::rotate(axis::X, angle * (std::numbers::pi / 180.0) * (1 - 2 * event.key.shift));
                } else if (event.key.code == sf::Keyboard::Y) {
                    return engine::Mat4::rotate(axis::Y, angle * (std::numbers::pi / 180.0) * (1 - 2 * event.key.shift));
                } else if (event.key.code == sf::Keyboard::Z) {
                    return engine::Mat4::rotate(axis::Z, angle * (std::numbers::pi / 180.0) * (1 - 2 * event.key.shift));
                }

                return engine::Mat4::identity();
            }();

            auto axis = [&]() -> engine::Vector_3D<T> {
                /*if (event.key.shift) {
                    return click;
                }*/
                return tetrahedron.vertex.back();
            }();

            for (auto & v : tetrahedron.vertex) {
                v = v * engine::Mat4::translate(0, -250, 0)
                        * transform * engine::Mat4::translate(0, 250, 0);
            }

            update_pixels();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                auto [x, y] = sf::Mouse::getPosition(window);
                click = {static_cast<T>(x), static_cast<T>(y)};
            }
        }
    }

    template <class W>
    auto render(W & window) -> void {
        window.draw(pixels_sprite);
    }

private:

    auto update_pixels() -> void {
        std::fill(std::begin(pixels), std::end(pixels), 255u);

        auto color = std::array<std::uint8_t, 4>{ 0, 255, 0, 255 };
        auto [p1, p2, p3, p4] = tetrahedron.vertex;

        engine::draw_triangle(pixels, engine::Triangle<T>{{ {{p1.x, p1.y}, {p2.x, p2.y}, {p3.x, p3.y}} }}, color);
        engine::draw_triangle(pixels, engine::Triangle<T>{{ {{p1.x, p1.y}, {p2.x, p2.y}, {p4.x, p4.y}} }}, color);
        engine::draw_triangle(pixels, engine::Triangle<T>{{ {{p2.x, p2.y}, {p3.x, p3.y}, {p4.x, p4.y}} }}, color);
        engine::draw_triangle(pixels, engine::Triangle<T>{{ {{p3.x, p3.y}, {p1.x, p1.y}, {p4.x, p4.y}} }}, color);

        pixels_texture.update(std::data(pixels));
    }
};

#endif //CPP_ENGINE_TETRAHEDRON_HPP
