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

#include "./shader/core.hpp"
#include "../gl-shaders/basic_vs.hpp"
#include "../gl-shaders/basic_fs.hpp"
#include "../gl-shaders/light_fs.hpp"

#include "../geometry/2d/vector.hpp"
#include "../io/obj_reader.hpp"
#include "../rng/core.hpp"

#include "../texture/core.hpp"

#include "./Entity_Base.hpp"
#include "./Model.hpp"
#include "./Entity_Owner.hpp"
#include "./Solid_Sphere.hpp"

#include "./md2/Model.hpp"

#include "../model/Vbo_Grid.hpp"
#include "../gl-shaders/grid_vs.hpp"

namespace engine {

template <Dim3_Vec U, Dim3_Vec V, class F>
auto sphere_sphere(U const& u, F r1, V const& v, F r2) -> bool {
    return distance_squared(u, v) < std::pow(r1 + r2, 2);
};

auto sphere_sphere(Solid_Sphere const* a, Solid_Sphere const* b) -> bool {
    return sphere_sphere(a->m_position, a->m_radius, b->m_position, b->m_radius);
};

auto perspective(float fov, float aspect, float near, float far) {
    auto bottom = -near * std::tan(fov / 2.0f);
    auto top = -bottom;
    auto left = aspect * bottom;
    auto right = -left;
    glFrustum(left, right, bottom, top, near, far);
}

template <class Vec, class Size>
auto to_gl_coordinates(Vec && vec, Size width, Size height) -> glm::vec2 {
    return glm::vec2{
        -1.0f + 2.0f * vec.x / width,
        1.0 - 2.0f * vec.y / height
    };
}

class Scene {
    using Window_Ptr = std::unique_ptr<sf::RenderWindow>;
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

    /* Camera */
    glm::vec2 m_camera_angle;
    glm::mat4 m_view;

    /* Shaders */
    std::uint32_t m_shader_main;
    std::uint32_t m_shader_grid;
    std::uint32_t m_shader_light;

    /* Md2 Sprite */
    std::int32_t m_md2_current;

    /* Wave timer */
    float m_wave_timer;
    float m_wave_intensity;
    float m_wave_strength;
    float m_wave_duration;
    glm::vec2 m_wave_center;

    /* Constructor/Init */
    explicit Scene(std::size_t width, std::size_t height, const char* name) :
            m_window(std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), name)),
            m_clock(),
            m_width(width),
            m_height(height),
            rng(make_default_generator<float>()),
            m_entities(),
            m_collision_tracker(),
            m_update(true),
            m_camera_angle({ 120.0f, 0.0f }),
            m_view(),
            m_shader_main(0),
            m_shader_grid(0),
            m_shader_light(0),
            m_md2_current(0),
            m_wave_timer(0.0f),
            m_wave_intensity(0.0f),
            m_wave_strength(0.6f),
            m_wave_duration(3.0f),
            m_wave_center({ -1.0f, -1.0f })
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
        create_shader_program();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //perspective(45.0f, m_height / m_width, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnable(GL_TEXTURE_2D);

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glDepthRange(0.0f, 1.0f);

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
                    m_width = event.size.width;
                    m_height = event.size.height;
                }
                else if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space) {
                        m_update = !m_update;
                    }

                    if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left) {
                        if (event.key.code == sf::Keyboard::Right) {
                            m_md2_current = (m_md2_current + 1) % (std::size(md2::Model_Sprints) - 1);
                        }
                        if (event.key.code == sf::Keyboard::Left) {
                            --m_md2_current;
                            if (m_md2_current < 0) {
                                m_md2_current = std::size(md2::Model_Sprints) - 2;
                            }
                        }
                        auto ptr = dynamic_cast<md2::Model*>(m_entities.front().get());
                        ptr->set_sprint_key(md2::Model_Sprints[m_md2_current]);
                    }

                    if (event.key.code == sf::Keyboard::Numpad8) {
                        m_camera_angle.x += 10.0f;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad6) {
                        m_camera_angle.y += 10.0f;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad2) {
                        m_camera_angle.x -= 10.0f;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad4) {
                        m_camera_angle.y -= 10.0f;
                    }
                }
                else if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        m_wave_center = to_gl_coordinates(sf::Mouse::getPosition(*m_window), m_width, m_height);
                        m_wave_intensity = m_wave_strength;
                        m_wave_timer = 0.0f;
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
        m_view = glm::rotate(glm::radians(m_camera_angle.x), glm::vec3{ 1.0f, 0.0f, 0.0f })
                    * glm::rotate(glm::radians(m_camera_angle.y), glm::vec3{ 0.0f, 1.0f, 0.0f });

        update_wave(elapsed);
        update_shaders();

        std::ranges::for_each(m_entities, [elapsed = elapsed.asSeconds()](auto const& e) { e->update(elapsed); });
        //handle_collisions();
    }

    auto update_wave(sf::Time const& elapsed) -> void {
        m_wave_timer += elapsed.asSeconds();
        m_wave_intensity -= elapsed.asSeconds() * (m_wave_strength / m_wave_duration);

        if (m_wave_intensity <= 0.0f) {
            m_wave_timer = 0.0f;
            m_wave_intensity = 0.0f;
        }
    }

    auto render() -> void {
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.95112f, 0.95112f, 0.95112f, 1.0f);

        glUseProgram(m_shader_light);

        std::ranges::for_each(m_entities, [](auto const& e){ e->render(); });
    }

    auto create_shader_program() -> void {
        auto vs_source = engine::assets::gl_shaders::basic_vs_source;
        auto fs_source = engine::assets::gl_shaders::basic_fs_source;

        auto grid_source = engine::assets::gl_shaders::grid_vs_source;

        auto light_source = engine::assets::gl_shaders::light_fs_source;

        m_shader_main = engine::shader::create_program(vs_source, fs_source);
        m_shader_grid = engine::shader::create_program(grid_source, fs_source);
        m_shader_light = engine::shader::create_program(vs_source, light_source);
    }

    auto update_shaders() -> void {
        glUniformMatrix4fv(glGetUniformLocation(m_shader_grid, "view"), 1, GL_FALSE, glm::value_ptr(m_view));

        glUniform1f(glGetUniformLocation(m_shader_grid, "timer"), m_wave_timer);
        glUniform1f(glGetUniformLocation(m_shader_grid, "intensity"), m_wave_intensity);

        glUniform2f(glGetUniformLocation(m_shader_grid, "center"), m_wave_center.x, m_wave_center.y);
    }

    auto spawn() -> void {
        //spawn_spheres();
        //spawn_wv_vbos();
        spawn_md2_vbos();
        //spawn_water();
    }

    auto spawn_wv_vbos() -> void {
        auto model = std::make_shared<Model>(io::read_wavefront<float>("../../wv-obj/tank-i.obj"));
        model->load();

        auto e1 = std::make_shared<Entity_Owner>(model, Vector_3Df{ -0.5f,   -0.5f,  0.1f });

        auto model2 = std::make_shared<Model>(io::read_wavefront<float>("../../wv-obj/orc.obj"));
        model2->load();

        auto e2 = std::make_shared<Entity_Owner>(model2, Vector_3Df{ 0.5f,   -0.5f,  0.1f });

        m_entities.push_back(e1);
        m_entities.push_back(e2);
    }

    auto spawn_md2_vbos() -> void {
        auto resource = md2::io::read("../../md2-obj/cathos.md2", "../../md2-obj/cathos.png");

        auto model = std::make_shared<md2::Model>(std::move(resource), md2::Model_Sprints[0], m_shader_main);
        model->push_gpu();

        m_entities.push_back(model);
    }

    auto spawn_water() -> void {
        auto grid = std::make_shared<Vbo_Grid>(200, 200, 0.01f, 0.01f, Texture("../../assets/images/water-texture.png", 0));
        grid->load();

        m_entities.push_back(grid);
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
