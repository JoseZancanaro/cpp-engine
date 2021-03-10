#include <fmt/core.h>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

auto main() -> int {
    auto window = sf::RenderWindow{ sf::VideoMode(800, 600), "cpp-engine" };
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);

    auto render_imgui= false;
    auto debug_window = [&]() mutable {
        ImGui::Begin("Debug");

        ImGui::End();
    };

    auto clock = sf::Clock{};

    while (window.isOpen()) {
        auto event = sf::Event{};

        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F12) {
                    render_imgui = !render_imgui;
                    fmt::print("{}\n", render_imgui);
                }
            }
        }

        ImGui::SFML::Update(window, clock.restart());
        window.clear(sf::Color(0, 116, 217));

        if (render_imgui) {
            debug_window();
        }

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
