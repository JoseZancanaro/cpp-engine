#ifndef CPP_ENGINE_MODEL_HPP
#define CPP_ENGINE_MODEL_HPP

#include <array>
#include <numbers>
#include <vector>
#include <utility>

#include <fmt/format.h>

#include "../gl.hpp"

#include "./Buffered_Entity_Base.hpp"
#include "../geometry/core.hpp"

namespace engine {

class Model : public Buffered_Entity_Base {
protected:
    /* vertex & index handle, dont use color */
    std::array<std::uint32_t, 2> m_vbo_handles;

    /* 3d m_solid data */
    std::uint32_t m_vertex_count;
    std::uint32_t m_indexes_count;
    std::vector<float> m_vertex;
    std::vector<std::uint16_t> m_indexes;

    float m_angle;

public:
    Model(Solid<float> const& solid) :
     m_vbo_handles(),
     m_vertex_count(std::size(solid.vertex)),
     m_indexes_count(),
     m_vertex(m_vertex_count * 3),  // vertex count * (x y z)
     m_indexes(), // indexes count * quad
     m_angle()
    {
        /* Copy contiguous vertex vector */
        std::memcpy(std::data(m_vertex), std::data(solid.vertex), sizeof(float) * 3 * m_vertex_count);

        /* Copy non-contiguous indexes vector as triangles */
        for (auto i = 0ul; i < m_indexes_count; ++i) {
            if (auto length = std::size(solid.faces[i].indexes); length == 3) {
                // Already a triangle
                m_indexes.push_back(solid.faces[i].indexes[0]);
                m_indexes.push_back(solid.faces[i].indexes[1]);
                m_indexes.push_back(solid.faces[i].indexes[2]);
            }
            else (length == 4) { 
                m_indexes.push_back(solid.faces[i].indexes[0]);
                m_indexes.push_back(solid.faces[i].indexes[1]);
                m_indexes.push_back(solid.faces[i].indexes[2]);

                m_indexes.push_back(solid.faces[i].indexes[0]);
                m_indexes.push_back(solid.faces[i].indexes[2]);
                m_indexes.push_back(solid.faces[i].indexes[3]);
            }
        }
    }

    auto load() -> void override {
        glGenBuffers(2, std::data(m_vbo_handles));

        /* vertex */
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_handles[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * std::size(m_vertex),
                                                std::data(m_vertex), GL_STATIC_DRAW);

        /* indexes */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo_handles[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(std::uint16_t) * std::size(m_indexes),
                                                std::data(m_indexes), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    };

    auto draw() -> void override {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glLoadIdentity();
        glScalef(0.01f, 0.01f, 0.01f);
        glRotatef(angle, 0.0f, 1.0f, 0.0f);
        glColor3f(1.0f, 0.0f, 0.0f);

        glEnableClientState(GL_VERTEX_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_handles[0]);
        glVertexPointer(3, GL_FLOAT, 0, 0l);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo_handles[1]);
        glDrawElements(GL_TRIANGLES, std::size(m_indexes), GL_UNSIGNED_SHORT, 0l);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDisableClientState(GL_VERTEX_ARRAY);

        glPopMatrix();
    };

    auto update(float seconds) -> void override {
        angle += 15.0f * std::numbers::pi_v<float> / 180.0f * seconds * 60.0f;
    }

    ~Model() override = default;
};

}

#endif //CPP_ENGINE_MODEL_HPP
