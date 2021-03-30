#include <fmt/core.h>
#include <fmt/ranges.h>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "sample/rectangle.hpp"
#include "sample/tetrahedron.hpp"

auto main() -> int {
    auto video_mode = sf::VideoMode(800, 600);
    auto window = sf::RenderWindow{video_mode, "cpp-engine"};

    ImGui::SFML::Init(window);
    auto clock = sf::Clock{};

    /* Instantiate template runner */
    auto runner = Tetrahedron_Runner<1920, 1080>{};

    while (window.isOpen()) {
        auto event = sf::Event{};

        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            /* Feed event */
            runner.event_hook(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, clock.restart());

        /* Request rendering */
        runner.render(window);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
