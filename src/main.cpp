#include <ranges>

#include <fmt/core.h>

#include <SFML/Graphics.hpp>
#include <SFMl/OpenGL.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include "sample/glwavefront_runner.hpp"
#include "io/obj_reader.hpp"

auto main() -> int {
    constexpr auto WIDTH = 1024, HEIGHT = 768;

    auto video_mode = sf::VideoMode(WIDTH, HEIGHT);
    auto window = sf::Window{video_mode, "cpp-engine"};
    window.setFramerateLimit(60);
    window.setActive(true);

    //ImGui::SFML::Init(window);

    auto clock = sf::Clock{};
    auto wv_object = engine::io::read_wavefront<float>("../../wv-obj/tank-i.obj"); // [samples at root/wv-obj]

    glEnable(GL_TEXTURE_2D);

    /* Instantiate template runner */
    auto runner = Gl_Wavefront_Runner<WIDTH, HEIGHT, float>(std::move(wv_object));

    bool running = true;
    while (running) {
        auto event = sf::Event{};

        while (window.pollEvent(event)) {
            //ImGui::SFML::ProcessEvent(event);

            /* Feed event */
            runner.event_hook(window, event);

            if (event.type == sf::Event::Closed) {
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        //ImGui::SFML::Update(window, clock.restart());

        /* Request rendering */
        runner.render(window);

        //ImGui::SFML::Render(window);
        window.display();
    }

    //ImGui::SFML::Shutdown();

    return 0;
}
