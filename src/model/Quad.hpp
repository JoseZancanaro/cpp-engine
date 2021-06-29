#ifndef CPP_ENGINE_MODEL_QUAD_HPP
#define CPP_ENGINE_MODEL_QUAD_HPP

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

#include "../gl.hpp"
#include "../scene/Buffered_Entity_Base.hpp"
#include "../texture/core.hpp"

namespace engine {

class Simple_Quad : public Buffered_Entity_Base {
    std::vector<glm::vec3> m_vertex_data;
    std::vector<glm::vec2> m_uv_data;
    std::vector<glm::vec3> m_tangent_data;
    std::vector<glm::vec3> m_bit_tangent_data;
    Texture m_texture;

    std::uint32_t m_vao;
    std::uint32_t m_vertex_vbo;
    std::uint32_t m_uv_vbo;
    std::uint32_t m_tangent_vbo;
    std::uint32_t m_bit_tangent_vbo;

public:
    Simple_Quad(std::vector<glm::vec3> && vertices, Texture && texture)
    : m_vertex_data(std::move(vertices)),
      m_uv_data(),
      m_tangent_data(),
      m_bit_tangent_data(),
      m_texture(std::move(texture)),
      m_vao(0),
      m_vertex_vbo(0),
      m_uv_vbo(0),
      m_tangent_vbo(0),
      m_bit_tangent_vbo(0)
    {}

    auto load() -> void override {

    }

};

}

#endif //CPP_ENGINE_MODEL_QUAD_HPP
