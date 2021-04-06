#ifndef CPP_ENGINE_WAVEFRONT_RUNNER_HPP
#define CPP_ENGINE_WAVEFRONT_RUNNER_HPP

#include <numbers>
#include <array>
#include <utility>

#include <fmt/core.h>

#include <SFML/Graphics.hpp>

#include "../geometry/core.hpp"
#include "../pixel-buffer.hpp"
#include "../draw.hpp"

template <std::size_t width, std::size_t height, class T = double>
class Wavefront_Runner {
    engine::Basic_RGBA_Buffer pixels;

    sf::Texture pixels_texture;
    sf::Sprite pixels_sprite;

    /* Solid data */
    engine::Solid<T> solid;

    /* Transform data */
    T angle_x;
    T angle_y;
    T angle_z;
    T angle_step;
    T cp; // communist party
    T cp_step;
    T move_x;
    T move_y;
    T move_step;
    T scale;
    T scale_step;

public:
    Wavefront_Runner(engine::Solid<T> && p_solid) :
        pixels(width, height, 255u),
        pixels_texture{},
        pixels_sprite{},
        solid{std::move(p_solid)},
        angle_x{0},
        angle_y{0},
        angle_z{0},
        angle_step{15},
        cp{500},
        cp_step{50},
        move_x{offset_to(width / 2.0, solid.vertex.front().x)},
        move_y{offset_to(height / 2.0, solid.vertex.front().y)},
        move_step{50},
        scale{1},
        scale_step{0.2}
    {
        pixels_texture.create(width, height);
        pixels_texture.update(std::data(pixels));
        pixels_sprite.setTexture(pixels_texture);
    }

    template <class W, class E>
    auto event_hook(W const& window, E event) -> void {
        if (event.type == sf::Event::KeyPressed) {
            /* Translate */
            if (event.key.code == sf::Keyboard::Up) {
                move_y -= move_step;
            } else if (event.key.code == sf::Keyboard::Down) {
                move_y += move_step;
            } else if (event.key.code == sf::Keyboard::Left) {
                move_x -= move_step;
            } else if (event.key.code == sf::Keyboard::Right) {
                move_x += move_step;
            }

            /* Scale */
            if (event.key.code == sf::Keyboard::Space) {
                scale += scale_step * (1 - 2 * event.key.shift);
            }

            /* Rotate */
            if (event.key.code == sf::Keyboard::X) { /* X */
                angle_x += angle_step * (1 - 2 * event.key.shift);
            }
            if (event.key.code == sf::Keyboard::Y) { /* Y */
                angle_y += angle_step * (1 - 2 * event.key.shift);
            }
            if (event.key.code == sf::Keyboard::Z) { /* Z */
                angle_z += angle_step * (1 - 2 * event.key.shift);
            }

            /* Update perspective */
            if (event.key.code == sf::Keyboard::Add) {
                cp += cp_step;
            } else if (event.key.code == sf::Keyboard::Subtract && (cp - cp_step) > 0) {
                cp -= cp_step;
            }

            auto transform = (
                    /* translate */
                    engine::Mat4::translate(move_x, move_y, 0) *
                    /* scale */
                    engine::Mat4::scale(scale, -scale, scale) * // negative, so y mirrors
                    /* projection */
                    engine::Mat4::simple_perspective(cp) *
                    /* perspective */
                    engine::Mat4::translate(-50, -50, 100) *
                    /* move forth */
                    //engine::Mat4::translate(0, 250, 0) *
                    /* rotate angle */
                    engine::Mat4::rotate(axis::Z, angle_z * (std::numbers::pi / 180.0)) *
                    engine::Mat4::rotate(axis::Y, angle_y * (std::numbers::pi / 180.0)) *
                    engine::Mat4::rotate(axis::X, angle_x * (std::numbers::pi / 180.0))// *
                    /* move to origin */
                    //engine::Mat4::translate(0, -250, 0)
            );

            auto solid_copy = solid;
            for (auto & v : solid_copy.vertex) {
                v = v * transform;
            }

            update_pixels(solid_copy);
        }
    }

    template <class W>
    auto render(W & window) -> void {
        window.draw(pixels_sprite);

        /*ImGui::Begin("Debug");
        ImGui::InputDouble("Massa", &angle_step);
        ImGui::End();*/
    }

private:
    constexpr auto offset_to(T ref, T target) -> T {
        return ref - target;
    }

    template <class Shape>
    auto update_pixels(Shape const& shape) -> void {
        std::fill(std::begin(pixels), std::end(pixels), 255u);

        auto color = std::array<std::uint8_t, 4>{ 0, 0, 0, 255 };
        engine::draw_solid(pixels, shape, color);

        pixels_texture.update(std::data(pixels));
    }
};

#endif //CPP_ENGINE_WAVEFRONT_RUNNER_HPP
