#ifndef CPP_ENGINE_MD2_MODEL_HPP
#define CPP_ENGINE_MD2_MODEL_HPP

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "../../gl.hpp"

#include "./header.hpp"
#include "./loader.hpp"
#include "../Entity_Base.hpp"

namespace engine::md2 {

class Model2 : public Entity_Base {
public:
    using Vertex_Vec = std::vector<glm::vec3>;
    using Frame_Vec = std::vector<Vertex_Vec>;

protected:
    Resource m_resource;
    std::uint32_t m_num_mesh;
    Frame_Vec m_vertex_frames;
    Frame_Vec m_normal_frames;

    /* GPU resources */
    std::uint32_t m_vao;
    std::vector<std::uint32_t> m_vertex_vbos;
    std::vector<std::uint32_t> m_normal_vbos;

    std::uint32_t m_next_vertex_frame_vbo;
    std::uint32_t m_next_normal_frame_vbo;

    bool update;

public:
    Model2(std::string const& p)
     : m_resource(io::read(p)),
       m_num_mesh(m_resource.num_mesh * 3),
       m_vertex_frames(Frame_Vec(m_resource.num_frames, Vertex_Vec(m_num_mesh))),
       m_normal_frames(Frame_Vec(m_resource.num_frames, Vertex_Vec(m_num_mesh))),
       m_vao(0),
       m_vertex_vbos(m_resource.num_frames),
       m_normal_vbos(m_resource.num_frames),
       m_next_vertex_frame_vbo(0),
       m_next_normal_frame_vbo(0)
    {
        fill_frame_vectors();
    }

    auto push_gpu() {
        glGenVertexArrays(1, &m_vao);

        glGenBuffers(std::size(m_vertex_vbos), std::data(m_vertex_vbos));
        glGenBuffers(std::size(m_normal_vbos), std::data(m_normal_vbos));

        glBindVertexArray(m_vao);

        for (auto i = 0ul; i < std::size(m_vertex_vbos); ++i) {
            glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbos[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_num_mesh, &m_vertex_frames[i][0], GL_DYNAMIC_DRAW);
        }

        for (auto i = 0ul; i < std::size(m_normal_vbos); ++i) {
            glBindBuffer(GL_ARRAY_BUFFER, m_normal_vbos[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_num_mesh, &m_normal_frames[i][0], GL_DYNAMIC_DRAW);
        }

        glBindVertexArray(0);
    }

    auto draw() -> void override {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glLoadIdentity();
        glScalef(1.0f, 1.0f, 1.0f);
        glColor3f(1.0f, 0.0f, 0.0f);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbos[m_resource.current_frame]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, m_num_mesh);

        glBindVertexArray(0);

        glPopMatrix();
    }

private:
    auto fill_frame_vectors() -> void {
        for (auto i = 0ul; i < m_resource.num_frames; ++i) {
            for (auto j = 0ul; j < m_resource.num_points; j += 3) {
                m_vertex_frames[i][j + 0] = m_resource.point[i * m_resource.num_points + m_resource.mesh[j].vec_index[0]];
                m_vertex_frames[i][j + 1] = m_resource.point[i * m_resource.num_points + m_resource.mesh[j].vec_index[1]];
                m_vertex_frames[i][j + 2] = m_resource.point[i * m_resource.num_points + m_resource.mesh[j].vec_index[2]];

                m_normal_frames[i][j + 0] = glm::normalize(m_resource.point[i * m_resource.num_points + m_resource.mesh[j].vec_index[0]]);
                m_normal_frames[i][j + 1] = glm::normalize(m_resource.point[i * m_resource.num_points + m_resource.mesh[j].vec_index[1]]);
                m_normal_frames[i][j + 2] = glm::normalize(m_resource.point[i * m_resource.num_points + m_resource.mesh[j].vec_index[2]]);
            }
        }
    }
};

}

#endif //CPP_ENGINE_MD2_MODEL_HPP
