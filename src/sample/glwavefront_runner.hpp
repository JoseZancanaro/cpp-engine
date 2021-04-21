#ifndef CPP_ENGINE_GLWAVEFRONT_RUNNER_HPP
#define CPP_ENGINE_GLWAVEFRONT_RUNNER_HPP

#include <ranges>
#include <type_traits>
#include <utility>

#include <fmt/core.h>

#include <SFML/Graphics.hpp>
#include <SFMl/OpenGL.hpp>

#include "../geometry/core.hpp"
#include "../pixel-buffer.hpp"
#include "../draw.hpp"

template <std::size_t width, std::size_t height, class T = double>
class Gl_Wavefront_Runner {
    using Vertex = std::add_lvalue_reference_t<decltype(engine::Solid<T>::vertex)>;
    using MinMax = std::invoke_result_t<decltype(std::ranges::minmax_element), Vertex, engine::less_t<axis::X_t>>;

    /* Solid data */
    engine::Solid<T> solid;

    /* Transform data */
    /* Bound. Box  */ MinMax bb_x, bb_y, bb_z;
    /* Centroid    */ T center_x, center_y, center_z;
    /* Angle       */ T angle;

public:
    Gl_Wavefront_Runner(engine::Solid<T> && p_solid) :
            solid{std::move(p_solid)},
            bb_x{std::ranges::minmax_element(solid.vertex, engine::less(axis::X))},
            bb_y{std::ranges::minmax_element(solid.vertex, engine::less(axis::Y))},
            bb_z{std::ranges::minmax_element(solid.vertex, engine::less(axis::Z))},
            center_x{std::midpoint(bb_x.max->x, bb_x.min->x)},
            center_y{std::midpoint(bb_y.max->y, bb_y.min->y)},
            center_z{std::midpoint(bb_z.max->z, bb_z.min->z)},
            angle{T(0)}
    {}

    template <class W, class E>
    auto event_hook(W const& window, E event) -> void {}

    template <class W>
    auto render(W & window) -> void {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glLoadIdentity();
        glRotatef(angle, 0.0f, 1.0f, 0.0f);
        glScalef(0.05f, 0.05f, 0.05f);

        glBegin(GL_TRIANGLES);

        for (auto i = 0ul; i < std::size(solid.faces); ++i) {
            auto const& face = solid.faces.at(i);
            auto n = std::size(face.indexes);

            if (n == 3) {
                auto v1 = solid.vertex.at(face.indexes.at(0) - 1);
                auto v2 = solid.vertex.at(face.indexes.at(1) - 1);
                auto v3 = solid.vertex.at(face.indexes.at(2) - 1);

                glColor3f(0.0f, 0.0f, 1.0f);
                glVertex3f(v1.x, v1.y, v1.z);
                glColor3f(0.0f, 0.0f, 1.0f);
                glVertex3f(v2.x, v2.y, v2.z);
                glColor3f(0.0f, 0.0f, 1.0f);
                glVertex3f(v3.x, v3.y, v3.z);
            }
            else if (n == 4) {
                auto v1 = solid.vertex.at(face.indexes.at(0) - 1);
                auto v2 = solid.vertex.at(face.indexes.at(1) - 1);
                auto v3 = solid.vertex.at(face.indexes.at(2) - 1);
                auto v4 = solid.vertex.at(face.indexes.at(3) - 1);

                glColor3f(0.0f, 0.0f, 1.0f);
                glVertex3f(v1.x, v1.y, v1.z);
                glColor3f(0.0f, 0.0f, 1.0f);
                glVertex3f(v2.x, v2.y, v2.z);
                glColor3f(0.0f, 0.0f, 1.0f);
                glVertex3f(v3.x, v3.y, v3.z);

                glColor3f(0.0f, 0.0f, 1.0f);
                glVertex3f(v3.x, v3.y, v3.z);
                glColor3f(0.0f, 0.0f, 1.0f);
                glVertex3f(v4.x, v4.y, v4.z);
                glColor3f(0.0f, 0.0f, 1.0f);
                glVertex3f(v1.x, v1.y, v1.z);
            }
        }

        ++angle;

        glEnd();
        glLoadIdentity();
    }

private:
    constexpr auto offset(T ref, T target) -> T {
        return ref - target;
    }
};

#endif //CPP_ENGINE_GLWAVEFRONT_RUNNER_HPP
