#include <fmt/core.h>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <numbers>
#include <array>

#include "geometry/core.hpp"
#include "draw.hpp"

auto main() -> int {
    auto video_mode = sf::VideoMode(800, 600);
    auto window = sf::RenderWindow{video_mode, "cpp-engine"};

    ImGui::SFML::Init(window);
    auto clock = sf::Clock{};

    auto pixels = engine::Basic_RGBA_Buffer{video_mode.width, video_mode.height, 255u};

    auto buffer_tex = sf::Texture{};
    buffer_tex.create(video_mode.width, video_mode.height);
    buffer_tex.update(std::data(pixels));

    auto buffer_sprite = sf::Sprite{buffer_tex};

    using precision_type = double;

    /* Transform data */
    auto rect = engine::Rectangle<precision_type>{{{ {100, 100}, {300, 100}, {300, 300}, {100, 300} }}};

    auto lastClick = engine::Vector_2D<precision_type> { 0, 0 };

    auto shift = 10.0;
    auto angle = 30.0;

    /* Update buffer */
    auto update_pixels = [&]() mutable {
        std::fill(std::begin(pixels), std::end(pixels), 255u);

        engine::draw_rect(pixels, rect, { 0, 255, 0, 255 });

        buffer_tex.update(std::data(pixels));
    };

    /* Debug window */
    auto render_imgui= false;
    auto debug_window = [&]() mutable {
        ImGui::Begin("Debug");

        ImGui::Text("%s", fmt::format("{} {} lastClick", lastClick.x, lastClick.y).c_str());

        ImGui::InputDouble("Shift (px)", &shift);
        ImGui::InputDouble("Angle (°)", &angle);

        if (ImGui::Button("Update")) {
            update_pixels();
        }

        ImGui::End();
    };

    update_pixels();

    while (window.isOpen()) {
        auto event = sf::Event{};

        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F12) {
                    render_imgui = !render_imgui;
                }

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

                auto axis = [&]() -> engine::Vector_2D<precision_type> {
                    if (event.key.shift) {
                        return lastClick;
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
                    lastClick = {static_cast<precision_type>(x), static_cast<precision_type>(y)};
                }
            }
        }

        ImGui::SFML::Update(window, clock.restart());
        /* window.clear(sf::Color(0, 116, 217)); */

        if (render_imgui) {
            debug_window();
        }

        window.draw(buffer_sprite);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
