#include <fmt/core.h>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <algorithm>
#include <vector>
#include <array>

#include "geometry/core.hpp"
#include "pixel-buffer.hpp"


auto main() -> int {
    auto video_mode = sf::VideoMode(800, 600);
    auto window = sf::RenderWindow{video_mode, "cpp-engine" };

    ImGui::SFML::Init(window);
    auto clock = sf::Clock{};

    auto pixels = engine::Basic_RGBA_Buffer{ video_mode.width, video_mode.height, 255u };

    auto buffer_tex = sf::Texture{};
    buffer_tex.create(video_mode.width, video_mode.height);
    buffer_tex.update(pixels.elements.data());

    auto buffer_sprite = sf::Sprite{buffer_tex};

    auto draw_line = false;

    auto p1 = std::array{ 0, 0};
    auto p2 = std::array{ 5, 4};

    auto update_pixels = [&]() mutable {
        std::fill(std::begin(pixels.elements), std::end(pixels.elements), 255u);

        auto [x0, y0] = p1;
        auto [x1, y1] = p2;
        engine::bresenham_plot(pixels, { x0, y0 },{ x1, y1 }, { 0, 0, 0, 255 });

        buffer_tex.update(pixels.elements.data());
    };

    auto render_imgui= false;
    auto debug_window = [&]() mutable {
        ImGui::Begin("Debug");

        ImGui::InputInt2("P1", p1.data());
        ImGui::InputInt2("P2", p2.data());

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
            } else if (event.type == sf::Event::MouseButtonPressed) {
                draw_line = true;
                auto [x, y] = sf::Mouse::getPosition(window);
                p1 = {x, y};
            }
        }

        ImGui::SFML::Update(window, clock.restart());
        /* window.clear(sf::Color(0, 116, 217)); */

        if (draw_line) {
            auto [x, y] = sf::Mouse::getPosition(window);
            p2 = {x, y};

            update_pixels();
        }

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
