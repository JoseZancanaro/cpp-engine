#include <fmt/core.h>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "sample/rectangle.hpp"
#include "sample/tetrahedron.hpp"
#include "sample/perspective_tetrahedron.hpp"
#include "sample/wavefront_runner.hpp"

#include "io/obj_reader.hpp"

auto main() -> int {
    constexpr auto WIDTH = 1024, HEIGHT = 768;

    auto video_mode = sf::VideoMode(WIDTH, HEIGHT);
    auto window = sf::RenderWindow{video_mode, "cpp-engine"};

    ImGui::SFML::Init(window);

    auto clock = sf::Clock{};
    auto tank = engine::io::read_wavefront("../../wv-obj/zer0.obj"); // [samples at root/wv-obj]

    /* Instantiate template runner */
    auto runner = Wavefront_Runner<WIDTH, HEIGHT>(std::move(tank));

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
