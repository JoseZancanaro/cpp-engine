#ifndef CPP_ENGINE_SCENE_HPP
#define CPP_ENGINE_SCENE_HPP

#include <memory>
#include <ranges>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

#include <fmt/core.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "../io/obj_reader.hpp"
#include "../rng/core.hpp"

#include "./Entity_Base.hpp"
#include "./Solid_Sphere.hpp"

namespace engine {

class Scene {
    using Window_Ptr = std::unique_ptr<sf::Window>;

public:
    Window_Ptr m_window;
    sf::Clock m_clock;

    /* Size */
    std::size_t m_width;
    std::size_t m_height;

    /* System::Entities */
    Random_Generator<float> rng;
    std::vector<std::shared_ptr<Entity_Base>> m_entities;
    std::vector<std::int64_t> m_collision_tracker;

    /* System::Status */
    bool m_update;

    /* Constructor/Init */
    explicit Scene(std::size_t width, std::size_t height, const char* name) :
     m_window(std::make_unique<sf::Window>(sf::VideoMode(width, height), name)),
     m_clock(),
     m_width(width),
     m_height(height),
     rng(make_default_generator<float>()),
     m_entities(),
     m_collision_tracker(),
     m_update(true)
    {
        load();
    }

    /* Main loop */
    auto start() -> void {
        m_window->setActive(true);
        event_loop();
    }

private:
    auto load() -> void {
        glViewport(0, 0, m_width, m_height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        spawn();
    }

    auto spawn() -> void {
        auto random_sphere = [rng = rng]() {
            auto sphere = std::make_shared<Solid_Sphere>(0.02f, 16, 16);
            sphere->set_position(rng(), rng(), 0.1f);
            sphere->set_color({ rng(), rng(), rng() });
            sphere->set_movement(rng() * 0.3f + 0.7f, normalize(Vector_3Df{ rng(), rng(), 0 }));

            return sphere;
        };

        for (auto i = 0ul; i < 50; ++i) {
            m_entities.push_back(random_sphere());
        }

        /*
        auto left = std::make_shared<Solid_Sphere>(0.08f, 16, 16);
        left->set_position(-0.5f, 0.0f, 0.1f);
        left->set_color({ 0.5f, 0.5f, 0.5f });
        left->set_movement(1.0f, { 1.0f, 0.0f, 0.0f });

        auto right = std::make_shared<Solid_Sphere>(0.08f, 16, 16);
        right->set_position(0.5f, 0.0f, 0.1f);
        right->set_color({ 0.9f, 0.9f, 0.9f });
        right->set_movement(1.0f, { -1.0f, 0.0f, 0.0f });

        m_entities.push_back(left);
        m_entities.push_back(right);
        */

        m_collision_tracker.resize(std::size(m_entities));
        std::ranges::fill(m_collision_tracker, -1);
    }

    auto event_loop() -> void {
        m_clock.restart();
        bool running = true;
        while (running) {
            auto event = sf::Event{};

            while (m_window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    running = false;
                }
                else if (event.type == sf::Event::Resized)
                {
                    glViewport(0, 0, event.size.width, event.size.height);
                }
                else if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space) {
                        m_update = !m_update;
                    }
                }
            }

            auto current_time = m_clock.restart();

            if (m_update) {
                update(current_time);
            }
            render();

            m_window->display();
        }
    }

    auto update(sf::Time elapsed) -> void {
        std::ranges::for_each(m_entities, [elapsed = elapsed.asSeconds()](auto const& e) { e->update(elapsed); });
        handle_collisions();
    }

    auto render() -> void {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.74112f, 0.74112f, 0.74112f, 1.f);

        glPushMatrix();
        std::ranges::for_each(m_entities, [](auto const& e){ e->draw(); });
        glPopMatrix();

        glPushMatrix();
    }

    /* Assumes Entities are spheres from Solid_Sphere */
    auto handle_collisions() -> void {
        using namespace std::chrono_literals;

        auto collides = [](Solid_Sphere const* a, Solid_Sphere const* b) -> bool {
            auto distance = std::sqrt(std::pow(a->m_position.x - b->m_position.x, 2) +
                                std::pow(a->m_position.y - b->m_position.y, 2) +
                                std::pow(a->m_position.z - b->m_position.z, 2));

            return distance <= a->m_radius + b->m_radius;
        };

        /* Nice O(N²) loop */
        for (auto i = 0ul; i < std::size(m_entities); ++i) {
            auto left = dynamic_cast<Solid_Sphere*>(m_entities[i].get());
            bool collided = false;

            for (auto j = 0ul; j < std::size(m_entities); ++j) {
                if (i != j) {
                    auto right = dynamic_cast<Solid_Sphere*>(m_entities[j].get());

                    if (collides(left, right)) {
                        if (m_collision_tracker[i] != j) {
                            //fmt::print("[B] {} | {} {} {}!\n", left->m_speed, left->m_orientation.x, left->m_orientation.y, left->m_orientation.z);
                            left->m_orientation = { -left->m_orientation.x, -left->m_orientation.y, -left->m_orientation.z };
                            //right->m_orientation = { -right->m_orientation.x, -right->m_orientation.y, -right->m_orientation.z };
                            m_collision_tracker[i] = j;
                            //m_collision_tracker[j] = i;
                            collided = true;
                            //fmt::print("[A] {} | {} {} {}!\n", left->m_speed, left->m_orientation.x, left->m_orientation.y, left->m_orientation.z);
                        }
                    } else {
                        if (m_collision_tracker[i] == j) {
                            m_collision_tracker[i] = -1;
                        }
                    }
                }
            }
        }
    }
};

}

#endif //CPP_ENGINE_SCENE_HPP
