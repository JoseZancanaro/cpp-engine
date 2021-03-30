#ifndef CPP_ENGINE_RECTANGLE_HPP
#define CPP_ENGINE_RECTANGLE_HPP

#include <numbers>
#include <array>

#include <SFML/Graphics.hpp>

#include "../geometry/core.hpp"
#include "../pixel-buffer.hpp"
#include "../draw.hpp"

template <std::size_t width, std::size_t height, class T = double>
class Rectangle_Runner {
    engine::Basic_RGBA_Buffer pixels;

    sf::Texture pixels_texture;
    sf::Sprite pixels_sprite;

    /* Example data */
    engine::Rectangle<T> rect;
    engine::Vector_2D<T> click;
    T shift;
    T angle;

public:

    Rectangle_Runner() :
        pixels(width, height, 255u),
        pixels_texture{},
        pixels_sprite{},
        rect{ {{ {100, 100}, {300, 100}, {300, 300}, {100, 300} }} },
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
            auto transform = [&]() -> engine::Mat3 {
                /* Translate */
                if (event.key.code == sf::Keyboard::Left) {
                    return engine::Mat3::translate(-shift, 0);
                } else if (event.key.code == sf::Keyboard::Right) {
                    return engine::Mat3::translate(shift, 0);
                } else if (event.key.code == sf::Keyboard::Up) {
                    return engine::Mat3::translate(0, -shift);
                } else if (event.key.code == sf::Keyboard::Down) {
                    return engine::Mat3::translate(0, shift);
                }

                /* Rotate */
                if (event.key.code == sf::Keyboard::A) {
                    return engine::Mat3::rotate(angle * (std::numbers::pi / 180.0));
                } else if (event.key.code == sf::Keyboard::S) {
                    return engine::Mat3::rotate(-angle * (std::numbers::pi / 180.0));
                }

                return engine::Mat3::identity();
            }();

            auto axis = [&]() -> engine::Vector_2D<T> {
                if (event.key.shift) {
                    return click;
                }
                return engine::centroid(rect);
            }();

            for (auto & v : rect.vertex) {
                auto origin = engine::translate(v, -axis.x, -axis.y);
                v = engine::translate(origin * transform, axis.x, axis.y);
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

        engine::draw_rect(pixels, rect, { 0, 255, 0, 255 });

        pixels_texture.update(std::data(pixels));
    }
};

#endif //CPP_ENGINE_RECTANGLE_HPP
