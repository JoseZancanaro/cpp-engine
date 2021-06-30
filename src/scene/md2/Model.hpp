#ifndef CPP_ENGINE_MD2_MODEL_HPP
#define CPP_ENGINE_MD2_MODEL_HPP

#include <string>
#include <vector>
#include <utility>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "../../gl.hpp"

#include <fmt/format.h>

#include "./header.hpp"
#include "./loader.hpp"
#include "../Entity_Base.hpp"

namespace engine::md2 {

class Model : public Entity_Base {
public:
    using Tex_Vertex = std::vector<glm::vec2>;
    using Vertex_Vec = std::vector<glm::vec3>;
    using Frame_Vec = std::vector<Vertex_Vec>;

protected:
    Resource m_resource;
    std::uint32_t m_num_points;
    Frame_Vec m_vertex_frames;
    Frame_Vec m_normal_frames;
    Tex_Vertex m_tex_vertex;
    Sprint_State m_state;

    /* GPU resources */
    std::uint32_t m_shader;
    std::uint32_t m_vao;
    std::vector<std::uint32_t> m_vertex_vbos;
    std::vector<std::uint32_t> m_normal_vbos;
    std::uint32_t m_tex_vbo;
    std::uint32_t m_tex_data_vbo;

    /* World resources */
    glm::mat4 m_transform;

    float m_fps;
    bool m_update;

public:
    template <class R>
    Model(R && r, Sprint_Key sk, std::uint32_t shader)
     : m_resource(std::forward<R>(r)),
       m_num_points(m_resource.num_mesh * 3),
       m_vertex_frames(Frame_Vec(m_resource.num_frames, Vertex_Vec(m_num_points))),
       m_normal_frames(Frame_Vec(m_resource.num_frames, Vertex_Vec(m_num_points))),
       m_tex_vertex(Tex_Vertex(m_num_points)),
       m_state{ sk, sk.first_frame, sk.first_frame + 1 },
       m_shader(shader),
       m_vao(0),
       m_vertex_vbos(m_resource.num_frames),
       m_normal_vbos(m_resource.num_frames),
       m_tex_vbo(0),
       m_tex_data_vbo(0),
       m_transform(glm::translate(glm::vec3{ 0.1f, 0.0f, 0.0f })
                    * glm::scale(glm::vec3{ 0.03f, 0.03f, 0.03f })
                    * glm::rotate(0.0f, glm::vec3{ 0.0f, 1.0f, 0.0f })),
       m_fps(10.0f),
       m_update(true)
    {
        fill_frame_vectors();
    }

    auto push_gpu() {
        glGenVertexArrays(1, &m_vao);

        glGenBuffers(std::size(m_vertex_vbos), std::data(m_vertex_vbos));
        glGenBuffers(std::size(m_normal_vbos), std::data(m_normal_vbos));

        glGenBuffers(1, &m_tex_vbo);

        glBindVertexArray(m_vao);

        for (auto i = 0ul; i < std::size(m_vertex_vbos); ++i) {
            glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbos[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_num_points, &m_vertex_frames[i][0], GL_DYNAMIC_DRAW);
        }

        for (auto i = 0ul; i < std::size(m_normal_vbos); ++i) {
            glBindBuffer(GL_ARRAY_BUFFER, m_normal_vbos[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_num_points, &m_normal_frames[i][0], GL_DYNAMIC_DRAW);
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_tex_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_num_points, &m_tex_vertex[0], GL_STATIC_DRAW);

        glGenTextures(1, &m_tex_data_vbo);
        glBindTexture(GL_TEXTURE_2D, m_tex_data_vbo);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_resource.tex_data.width, m_resource.tex_data.height, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, std::data(m_resource.tex_data.buffer));
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    auto render() -> void override {
        glPushMatrix();

        glLoadIdentity();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_tex_data_vbo);

        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, m_num_points);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glPopMatrix();
    }

    auto update(float seconds) -> void override {
        m_state.current_time += seconds;

        if (m_state.lerp >= 1.0f) {
            m_state.current_frame = m_state.next_frame;
            ++m_state.next_frame;
            if (m_state.current_frame == m_state.current_sprint.last_frame) {
                m_state.next_frame = m_state.current_sprint.first_frame;
            }
            m_state.lerp = 0.0f;
            m_state.old_time = m_state.current_time;
            m_state.current_time = 0.0f;
            m_update = true;
        }

        if (m_update) {
            glBindVertexArray(m_vao);

            glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbos[m_state.current_frame]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbos[m_state.next_frame]);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(3);

            glBindBuffer(GL_ARRAY_BUFFER, m_normal_vbos[m_state.current_frame]);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(2);

            glBindBuffer(GL_ARRAY_BUFFER, m_normal_vbos[m_state.next_frame]);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(4);

            glBindBuffer(GL_ARRAY_BUFFER, m_tex_vbo);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);

            m_update = false;
        }

        m_state.lerp += seconds * m_state.current_sprint.fps;

        glUniform1f(glGetUniformLocation(m_shader, "lerp"), m_state.lerp);
        glUniform1i(glGetUniformLocation(m_shader, "tex"), 0);
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "transform"), 1, GL_FALSE, glm::value_ptr(m_transform));
        glUniform3f(glGetUniformLocation(m_shader, "light"), 0.0f, 0.0f, 0.0f);
    }

    auto set_sprint_key(Sprint_Key sk) -> void {
        m_state.current_sprint = sk;
        m_state.current_frame = sk.first_frame;
        m_state.next_frame = sk.first_frame + 1;
        m_state.old_time = m_state.current_time;
        m_state.current_time = 0.0f;
        m_update = true;
    }

private:
    auto fill_frame_vectors() -> void {
        for (auto i = 0ul; i < m_resource.num_frames; ++i) {
            for (auto j = 0ul, idx = 0ul; j < m_resource.num_mesh; ++j, idx += 3) {
                m_vertex_frames[i][idx + 0] = m_resource.point[i * m_resource.num_points + m_resource.mesh[j].vec_index[0]];
                m_vertex_frames[i][idx + 1] = m_resource.point[i * m_resource.num_points + m_resource.mesh[j].vec_index[1]];
                m_vertex_frames[i][idx + 2] = m_resource.point[i * m_resource.num_points + m_resource.mesh[j].vec_index[2]];

                m_normal_frames[i][idx + 0] = glm::normalize(m_resource.point[i * m_resource.num_points + m_resource.mesh[j].vec_index[0]]);
                m_normal_frames[i][idx + 1] = glm::normalize(m_resource.point[i * m_resource.num_points + m_resource.mesh[j].vec_index[1]]);
                m_normal_frames[i][idx + 2] = glm::normalize(m_resource.point[i * m_resource.num_points + m_resource.mesh[j].vec_index[2]]);
            }
        }

        if (m_resource.num_frames > 0) {
            for (auto i = 0ul, idx = 0ul; i < m_resource.num_mesh; ++i, idx += 3) {
                m_tex_vertex[idx + 0] = m_resource.tex[m_resource.mesh[i].tex_index[0]];
                m_tex_vertex[idx + 1] = m_resource.tex[m_resource.mesh[i].tex_index[1]];
                m_tex_vertex[idx + 2] = m_resource.tex[m_resource.mesh[i].tex_index[2]];
            }
        }
    }
};

}

#endif //CPP_ENGINE_MD2_MODEL_HPP
