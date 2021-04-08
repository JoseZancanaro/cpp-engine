#ifndef CPP_ENGINE_WAVEFRONT_RUNNER_HPP
#define CPP_ENGINE_WAVEFRONT_RUNNER_HPP

#include <array>
#include <execution>
#include <numbers>
#include <numeric>
#include <ranges>
#include <type_traits>
#include <utility>

#include <fmt/core.h>

#include <SFML/Graphics.hpp>

#include "../geometry/core.hpp"
#include "../pixel-buffer.hpp"
#include "../draw.hpp"

template <std::size_t width, std::size_t height, class T = double>
class Wavefront_Runner {
    using Vertex = std::add_lvalue_reference_t<decltype(engine::Solid<T>::vertex)>;
    using MinMax = std::invoke_result_t<decltype(std::ranges::minmax_element), Vertex, engine::less_t<axis::X_t>>;

    /* Solid data */
    engine::Solid<T> solid;
    engine::Solid<T> view;

    /* Pixel data */
    engine::Basic_RGBA_Buffer pixels;
    sf::Texture pixels_texture;
    sf::Sprite pixels_sprite;

    /* Transform data */
    /* Bound. Box  */ MinMax bb_x, bb_y, bb_z;
    /* Centroid    */ T center_x, center_y, center_z;
    /* Click       */ T click_x, click_y;
    /* Angle       */ T angle_x, angle_y, angle_z, angle_step;
    /* Cent. Proj. */ T cp, cp_step;
    /* Movement    */ T move_x, move_y, move_step;
    /* Scale       */ T scale, scale_step;

    /* gui status */
    bool show_debug;

public:
    Wavefront_Runner(engine::Solid<T> && p_solid) :
            solid{std::move(p_solid)},
            view{solid},
            pixels(width, height, 255u), pixels_texture{}, pixels_sprite{},
            bb_x{std::ranges::minmax_element(solid.vertex, engine::less(axis::X))},
            bb_y{std::ranges::minmax_element(solid.vertex, engine::less(axis::Y))},
            bb_z{std::ranges::minmax_element(solid.vertex, engine::less(axis::Z))},
            center_x{std::midpoint(bb_x.max->x, bb_x.min->x)},
            center_y{std::midpoint(bb_y.max->y, bb_y.min->y)},
            center_z{std::midpoint(bb_z.max->z, bb_z.min->z)},
            click_x{}, click_y{},
            angle_x{0}, angle_y{0}, angle_z{0}, angle_step{15},
            cp{500}, cp_step{50},
            move_x{offset(width / 2.0, center_x)},
            move_y{offset(height / 2.0, center_x)},
            move_step{50},
            scale{1}, scale_step{0.5},
            show_debug{false}
    {
        pixels_texture.create(width, height);
        pixels_texture.update(std::data(pixels));
        pixels_sprite.setTexture(pixels_texture);

        fmt::print("Min [ {} {} {} ] Max [ {} {} {} ]\n", bb_x.min->x, bb_y.min->y, bb_z.min->z,
                   bb_x.max->x, bb_y.max->y, bb_z.max->z);
    }

    template <class W, class E>
    auto event_hook(W const& window, E event) -> void {
        if (event.type == sf::Event::KeyPressed) {
            auto near = 0.1, far = 1000.0, q = far / (far - near);
            auto aspect_ratio = height / double{width};

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

            /* Pivot Movement */
            auto [ pivot_x, pivot_y, pivot_z ] = [&]() -> engine::Vector_3D<T> {
                if (event.key.alt) return { click_x, click_y, 0 };
                return { 0, 0, 0 };
            }();

            /* GUI status */
            if (event.key.code == sf::Keyboard::F12) {
                show_debug = !show_debug;
            }

            auto transform = (
                    /* translate away from cp*/
                    //engine::Mat4::translate(-center_x, -center_y, -center_z) *
                    /* perspective */
                    //engine::Mat4::simple_perspective(near, q, aspect_ratio) *
                    /* move forth */
                    engine::Mat4::translate(center_x, center_y, center_z) *
                    /* translate to pivot */
                    engine::Mat4::translate(pivot_x, pivot_y, pivot_z) *
                    /* translate */
                    engine::Mat4::translate(move_x, move_y, 0) *
                    /* scale */
                    engine::Mat4::scale(scale, -scale, scale) * // mirror y [negative scale]
                    /* rotate angle */
                    engine::Mat4::rotate(axis::Z, angle_z * (std::numbers::pi / 180.0)) *
                    engine::Mat4::rotate(axis::Y, angle_y * (std::numbers::pi / 180.0)) *
                    engine::Mat4::rotate(axis::X, angle_x * (std::numbers::pi / 180.0)) *
                    /* move to origin */
                    engine::Mat4::translate(-center_x, -center_y, -center_z) *
                    /* noop */
                    engine::Mat4::identity()
            );

            std::transform(std::execution::par_unseq, std::cbegin(solid.vertex), std::cend(solid.vertex),
                            std::begin(view.vertex), [transform](auto const& v) { return v * transform; });

            update_pixels(view);
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                auto [ x, y ] = sf::Mouse::getPosition(window);
                click_x = offset(T(x), width / 2.0);
                click_y = offset(T(y), height / 2.0);
            }
        }
    }

    template <class W>
    auto render(W & window) -> void {
        window.draw(pixels_sprite);
        if (show_debug) {
            imgui();
        }
    }

private:
    constexpr auto offset(T ref, T target) -> T {
        return ref - target;
    }

    template <class Shape>
    auto update_pixels(Shape const& shape) -> void {
        std::fill(std::begin(pixels), std::end(pixels), 255u);

        auto color = std::array<std::uint8_t, 4>{ 0, 0, 0, 255 };
        engine::draw_solid(pixels, shape, color);

        pixels_texture.update(std::data(pixels));
    }

    auto imgui() -> void {
        ImGui::Begin("Debug");

        ImGui::TextColored({ 255, 255, 255, 255 }, "Vertex %lu Faces %lu", std::size(solid.vertex), std::size(solid.faces));
        ImGui::InputDouble("Angle step", &angle_step);
        ImGui::InputDouble("Scale step", &scale_step);
        ImGui::InputDouble("Move step", &move_step);

        ImGui::End();
    }
};

#endif //CPP_ENGINE_WAVEFRONT_RUNNER_HPP
