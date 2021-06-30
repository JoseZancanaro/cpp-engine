#ifndef CPP_ENGINE_MODEL_QUAD_HPP
#define CPP_ENGINE_MODEL_QUAD_HPP

#include <cstdint>
#include <pair>
#include <vector>

#include <glm/glm.hpp>

#include "../gl.hpp"
#include "../scene/Buffered_Entity_Base.hpp"
#include "../texture/core.hpp"

namespace engine {

auto tans(glm::vec3 const& i, glm::vec3 const& j, glm::vec3 const& k,
            glm::vec2 const& s, glm::vec2 const& t, glm::vec2 const& p) -> std::pair<glm::vec3, glm::vec3> {
    auto edge_ji = j - i, edge_ki = k - i;
    auto uv_ts = t - s, uv_ps = p - s;

    auto f = 1.0f / (uv_ts.x * uv_ps.y - uv_ps.x * uv_ts.y);

    return std::make_pair(
        glm::vec3(
                f * (uv_ps.y * edge_ji.x - uv_ts.y * edge_ki.x),
                f * (uv_ps.y * edge_ji.y - uv_ts.y * edge_ki.y),
                f * (uv_ps.y * edge_ji.z - uv_ts.y * edge_ki.z)),
        glm::vec3(
                f * (-uv_ps.x * edge_ji.x + uv_ts.x * edge_ki.x),
                f * (-uv_ps.x * edge_ji.y + uv_ts.x * edge_ki.y),
                f * (-uv_ps.x * edge_ji.z + uv_ts.x * edge_ki.z))
    );
}

class Simple_Quad : public Buffered_Entity_Base {
    std::vector<glm::vec3> m_vertex_data;
    std::vector<glm::vec3> m_normal_data;
    std::vector<glm::vec2> m_uv_data;
    std::vector<glm::vec3> m_tangent_data;
    std::vector<glm::vec3> m_bitangent_data;

    Texture m_texture_diffuse;
    Texture m_texture_normal;

    std::uint32_t m_vao;
    std::uint32_t m_vertex_vbo;
    std::uint32_t m_normal_vbo;
    std::uint32_t m_uv_vbo;
    std::uint32_t m_tangent_vbo;
    std::uint32_t m_bit_tangent_vbo;

public:
    Simple_Quad(std::vector<glm::vec3> && vertices, Texture && diffuse, Texture && normal)
    : m_vertex_data(std::move(vertices)),
      m_normal_data(std::size(m_vertex_data), glm::vec3(0.0f, 0.0f, 1.0f)),
      m_uv_data({ glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f) }),
      m_texture_diffuse(std::move(diffuse)),
      m_texture_normal(std::move(normal)),
      m_vao(0),
      m_vertex_vbo(0),
      m_normal_vbo(0),
      m_uv_vbo(0),
      m_tangent_vbo(0),
      m_bit_tangent_vbo(0)
    {
        auto [tan1, bitan1] = tans(m_vertex_data[0], m_vertex_data[1], m_vertex_data[2], m_uv_data[0], m_uv_data[1], m_uv_data[2]);
        auto [tan2, bitan2] = tans(m_vertex_data[0], m_vertex_data[2], m_vertex_data[3], m_uv_data[0], m_uv_data[2], m_uv_data[3]);

        m_tangent_data = {tan1, tan1, tan1, tan2, tan2, tan2};
        m_bitangent_data = {bitan1, bitan1, bitan1, bitan2, bitan2, bitan2};
    }

    auto load() -> void override {
        glGenVertexArrays(1, &m_vao);

        glGenBuffers(1, &m_vertex_vbo);
        glGenBuffers(1, &m_normal_vbo);
        glGenBuffers(1, &m_uv_vbo);
        glGenBuffers(1, &m_tangent_vbo);
        glGenBuffers(1, &m_bit_tangent_vbo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * std::size(m_vertex_data), std::data(m_vertex_data), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, m_normal_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * std::size(m_normal_data), std::data(m_normal_data), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, m_uv_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * std::size(m_uv_data), std::data(m_uv_data), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, m_tangent_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * std::size(m_tangent_data), std::data(m_tangent_data), GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, m_bitangent_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * std::size(m_bitangent_data), std::data(m_bitangent_data), GL_STATIC_DRAW);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(4);

        m_texture_diffuse.gen_buffer();
        m_texture_normal.gen_buffer()

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    auto render() -> void override {
        m_texture_diffuse.bind();
        //m_texture_normal.bind();

        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, std::size(m_vertex_data));
        glBindVertexArray(0);
    }
};

}

#endif //CPP_ENGINE_MODEL_QUAD_HPP
