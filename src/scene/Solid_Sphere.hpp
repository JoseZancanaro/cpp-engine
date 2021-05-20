#ifndef CPP_ENGINE_SOLID_SPHERE_HPP
#define CPP_ENGINE_SOLID_SPHERE_HPP

#include <cmath>
#include <vector>
#include <numbers>

#include <fmt/core.h>

#include "../gl.hpp"

#include "../geometry/3d/vector.hpp"
#include "./Entity_Base.hpp"

namespace engine {

class Solid_Sphere : public Entity_Base {
public:
    /* Coordinates */
    Vector_3Df m_position;
    float const m_radius;
    float m_speed;
    Vector_3Df m_orientation;
    Vector_3Df m_color;

protected:
    /* Sphere drawing */
    std::vector<float> m_vertices;
    std::vector<std::uint16_t> m_indices;

public:
    Solid_Sphere(float radius, std::uint32_t rings, std::uint32_t sectors) :
     m_position(), m_radius(radius), m_speed(), m_orientation(), m_color({ 1.0f, 1.0f, 1.0f }),
     m_vertices(rings * sectors * 3),
     m_indices(rings * sectors * 4)
    {
        constexpr auto PI = std::numbers::pi_v<float>;

        auto const R = 1.0f / static_cast<float>(rings - 1);
        auto const S = 1.0f / static_cast<float>(sectors - 1);

        auto v = std::begin(m_vertices);

        for(auto r = 0ul; r < rings; ++r) {
            for(auto s = 0ul; s < sectors; ++s) {
                float const i = std::sin(-(PI / 2) + PI * r * R);
                float const j = std::cos(2 * PI * s * S) * std::sin(PI * r * R);
                float const k = std::sin(2 * PI * s * S) * std::sin(PI * r * R);

                *v++ = j * radius;
                *v++ = i * radius;
                *v++ = k * radius;
            }
        }

        auto i = std::begin(m_indices);
        for(auto r = 0ul; r < rings - 1; ++r) {
            for(auto s = 0ul; s < sectors - 1; ++s) {
                *i++ = r * sectors + s;
                *i++ = r * sectors + (s + 1);
                *i++ = (r + 1) * sectors + (s + 1);
                *i++ = (r + 1) * sectors + s;
            }
        }
    }

    auto set_position(float x, float y, float z) -> void {
        m_position = { x, y, z };
    }

    auto set_position(Vector_3Df const& u) -> void {
        m_position = u;
    }

    auto set_movement(float speed, Vector_3Df const& orientation) -> void {
        m_speed = speed;
        m_orientation = orientation;
    }

    auto set_color(Vector_3Df const& color) -> void {
        m_color = color;
    }

    virtual auto draw() -> void override {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glColor3f(m_color.x, m_color.y, m_color.z);
        glTranslatef(m_position.x, m_position.y, m_position.z);

        glEnableClientState(GL_VERTEX_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, &m_vertices[0]);
        glDrawElements(GL_QUADS, std::size(m_indices), GL_UNSIGNED_SHORT, &m_indices[0]);

        glPopMatrix();
    }

    auto update(float seconds) -> void override {
        auto [x, y, z] = m_position;

        auto i = x + m_speed * m_orientation.x * seconds;
        auto j = y + m_speed * m_orientation.y * seconds;
        auto k = z + m_speed * m_orientation.z * seconds;

        if (i > 1.0f || i < -1.0f) {
            i = x;
            m_orientation = { -m_orientation.x, m_orientation.y, m_orientation.z };
        }

        if (j > 1.0f || j < -1.0f) {
            j = y;
            m_orientation = { m_orientation.x, -m_orientation.y, m_orientation.z };
        }

        if (k > 1.0f || k < -1.0f) {
            k = z;
            m_orientation = { m_orientation.x, m_orientation.y, -m_orientation.z };
        }

        m_position = { i, j, k };
    }

    ~Solid_Sphere() override = default;
};

}

#endif //CPP_ENGINE_SOLID_SPHERE_HPP
