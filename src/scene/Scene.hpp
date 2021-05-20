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
#include "../gl.hpp"

#include "../geometry/2d/vector.hpp"
#include "../io/obj_reader.hpp"
#include "../rng/core.hpp"

#include "./Entity_Base.hpp"
#include "./Model.hpp"
#include "./Entity_Owner.hpp"
#include "./Solid_Sphere.hpp"

namespace engine {

template <Dim3_Vec U, Dim3_Vec V, class F>
auto sphere_sphere(U const& u, F r1, V const& v, F r2) -> bool {
    return distance_squared(u, v) < std::pow(r1 + r2, 2);
};

auto sphere_sphere(Solid_Sphere const* a, Solid_Sphere const* b) -> bool {
    return sphere_sphere(a->m_position, a->m_radius, b->m_position, b->m_radius);
};

class Scene {
    using Window_Ptr = std::unique_ptr<sf::Window>;
    using Entity_Ptr = std::shared_ptr<Entity_Base>;

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
     m_update(false)
    {
        load();
    }

    /* Main loop */
    auto start() -> void {
        m_window->setFramerateLimit(60);
        m_window->setActive(true);
        event_loop();
    }

private:
    auto load() -> void {
        glewInit();
        glViewport(0, 0, m_width, m_height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        spawn();
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
        //handle_collisions();
    }

    auto render() -> void {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.95112f, 0.95112f, 0.95112f, 1.f);

        glPushMatrix();
        std::ranges::for_each(m_entities, [](auto const& e){ e->draw(); });
        glPopMatrix();

        glPushMatrix();
    }

    auto spawn() -> void {
        //spawn_spheres();
        spawn_vbos();
    }

    auto spawn_vbos() -> void {
        auto model = std::make_shared<Model>(io::read_wavefront<float>("../../wv-obj/tank-i.obj"));
        model->load();

        auto e1 = std::make_shared<Entity_Owner>(model, Vector_3Df{ 0.0f,   0.0f,  0.1f });
        auto e2 = std::make_shared<Entity_Owner>(model, Vector_3Df{ -0.25f, 0.25f, 0.1f });
        auto e3 = std::make_shared<Entity_Owner>(model, Vector_3Df{ 0.25f, -0.25f, 0.1f });

        m_entities.push_back(e1);
        m_entities.push_back(e2);
        m_entities.push_back(e3);
    }

    auto spawn_spheres() -> void {
        auto static_position_factory = []([[maybe_unused]] float radius) {
            return Vector_3Df { 0.0f, 0.0f, 0.1f };
        };

        auto position_factory = [rng = rng, &entities = m_entities](float radius) {
            auto would_collide = [&entities]<Dim3_Vec U, class F>(U const& u, F radius) {
                return std::ranges::any_of(entities, [&u, radius](Entity_Ptr entity) {
                    auto sphere = dynamic_cast<Solid_Sphere*>(entity.get());
                    return sphere_sphere(u, radius, sphere->m_position, sphere->m_radius);
                });
            };

            auto position = Vector_3Df{ 0.0f, 0.0f, 0.1f };
            do {
                position.x = rng() * 2 - 1;
                position.y = rng() * 2 - 1;
            } while (would_collide(position, radius));

            return position;
        };

        auto random_color = [rng = rng]() -> Vector_3Df {
            return { rng() * 0.86f, rng() * 0.86f, rng() * 0.86f };
        };

        auto random_sphere = [rng = rng, random_color, factory = static_position_factory]() {
            auto sphere = std::make_shared<Solid_Sphere>(rng() * 0.015f + 0.02f, 16, 16);
            sphere->set_position(factory(sphere->m_radius));
            sphere->set_color(random_color());
            sphere->set_movement(rng() * 0.3f + 0.7f, normalize(Vector_3Df{ rng() * 2 - 1, rng() * 2 - 1, 0 }));
            return sphere;
        };

        /* Setup fixed spheres */
        /* top_left moving bottom_right */
        auto left = std::make_shared<Solid_Sphere>(0.06f, 16, 16);
        left->set_position(-0.5f, 0.5f, 0.1f);
        left->set_color(random_color());
        left->set_movement(1.0f, normalize(Vector_3Df{ 1.0f, -1.0f, 0.0f }));

        /* bottom_right moving top_left */
        auto right = std::make_shared<Solid_Sphere>(0.04f, 16, 16);
        right->set_position(0.5f, -0.5f, 0.1f);
        right->set_color(random_color());
        right->set_movement(1.0f, normalize(Vector_3Df{ -1.0f, 1.0f, 0.0f }));

        m_entities.push_back(left);
        m_entities.push_back(right);

        /* Setup random spheres */
        for (auto i = 0ul; i < 100; ++i) {
            m_entities.push_back(random_sphere());
        }

        /* Setup collision tracker */
        m_collision_tracker.resize(std::size(m_entities));
        std::ranges::fill(m_collision_tracker, -1);
    }

    /* Assumes Entities are spheres from Solid_Sphere */
    auto handle_collisions() -> void {
        using namespace std::chrono_literals;

        auto simple_response = [](Solid_Sphere * left, Solid_Sphere * right) {
            auto project = []<Dim3_Vec U, Dim3_Vec V>(U const& u, V const& v) {
                return scale(v, dot_product(u, v) / dot_product(v, v));
            };

            auto v1 = left->m_orientation;
            auto v2 = right->m_orientation;

            left->set_movement(left->m_speed, v1 + project(v2, v2 - v1) - project(v1, v1 - v2));
            right->set_movement(right->m_speed, v2 + project(v1, v2 - v1) - project(v2, v1 - v2));
        };

        auto advanced_response = [](Solid_Sphere * left, Solid_Sphere * right) {
            auto v1 = left->m_orientation;
            auto v2 = right->m_orientation;

            auto delta = normalize(v1 - v2);
            auto x1 = dot_product(delta, v1);
            auto v1x = scale(delta, x1);
            auto v1y = v1 - v1x;
            auto m1 = left->m_radius;

            auto inv_delta = scale(delta, -1);
            auto x2 = dot_product(inv_delta, v2);
            auto v2x = scale(inv_delta, x2);
            auto v2y = v2 - v2x;
            auto m2 = right->m_radius;

            left->m_orientation = scale(v1x, (m1 - m2) / (m1 + m2)) + scale(v2x, (2 * m2) / (m1 + m2)) + v1y;
            right->m_orientation = scale(v1x, (2 * m1) / (m1 + m2)) + scale(v2x, (m1 - m2) / (m1 + m2)) + v2y;
        };

        /* Nice O(N²) loop */
        for (auto i = 1ul; i < std::size(m_entities); ++i) {
            auto left = dynamic_cast<Solid_Sphere*>(m_entities[i].get());

            for (auto j = 0ul; j < i; ++j) {
                auto right = dynamic_cast<Solid_Sphere*>(m_entities[j].get());

                if (sphere_sphere(left, right)) {
                    if (m_collision_tracker[i] != j && m_collision_tracker[j] != i) {
                        simple_response(left, right);

                        m_collision_tracker[i] = j;
                        m_collision_tracker[j] = i;
                    }
                } else {
                    if (m_collision_tracker[i] == j) { m_collision_tracker[i] = -1; }
                    if (m_collision_tracker[j] == i) { m_collision_tracker[j] = -1; }
                }
            }
        }
    }
};

}

#endif //CPP_ENGINE_SCENE_HPP
