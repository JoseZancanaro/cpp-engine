#ifndef CPP_ENGINE_MODEL_VBO_GRID_HPP
#define CPP_ENGINE_MODEL_VBO_GRID_HPP

#include <cstdint>
#include <vector>

#include "../gl.hpp"
#include "../scene/Buffered_Entity_Base.hpp"
#include "../texture/core.hpp"

namespace engine {

class Vbo_Grid : public Buffered_Entity_Base {
public:
    std::int32_t m_width;
    std::int32_t m_height;
    float m_cell_width;
    float m_cell_height;
    Texture m_texture;

    std::uint32_t m_vertex_vbo;
    std::uint32_t m_uv_vbo;

    std::uint32_t m_vertex_count;
    std::vector<float> m_vertex_data;
    std::vector<float> m_uv_data;

    Vbo_Grid(std::int32_t width, std::int32_t height, float cell_width, float cell_height, Texture && texture)
     : m_width(width),
       m_height(height),
       m_cell_width(cell_width),
       m_cell_height(cell_height),
       m_texture(std::move(texture)),
       m_vertex_vbo(0),
       m_uv_vbo(0),
       m_vertex_count((width - 1) * (height - 1) * 6),
       m_vertex_data(),
       m_uv_data()
    {}

    auto load() -> void override {
        using Mat_1 = std::vector<float>;
        using Mat_2 = std::vector<Mat_1>;
        using Mat_3 = std::vector<Mat_2>;

        auto grid = Mat_3(m_width, Mat_2(m_height, Mat_1(3, 0.0f)));

        for (auto i = 0ul; i < m_width; ++i) {
            for (auto j = 0ul; j < m_height; ++j) {
                grid[i][j][0] = static_cast<float>(i) * m_cell_width - 1.0f;
                grid[i][j][1] = static_cast<float>(j) * m_cell_height - 0.5f;
                grid[i][j][2] = 0.0f;
            }
        }

        m_vertex_data.reserve(m_vertex_count * 3);

        for (auto i = 0ul; i < m_width - 1; ++i) {
            for (auto j = 0ul; j < m_height - 1; ++j) {
                m_vertex_data.insert(std::end(m_vertex_data), std::cbegin(grid[i + 0][j + 0]), std::cend(grid[i + 0][j + 0]));
                m_vertex_data.insert(std::end(m_vertex_data), std::cbegin(grid[i + 1][j + 0]), std::cend(grid[i + 1][j + 0]));
                m_vertex_data.insert(std::end(m_vertex_data), std::cbegin(grid[i + 1][j + 1]), std::cend(grid[i + 1][j + 1]));

                m_vertex_data.insert(std::end(m_vertex_data), std::cbegin(grid[i + 1][j + 1]), std::cend(grid[i + 1][j + 1]));
                m_vertex_data.insert(std::end(m_vertex_data), std::cbegin(grid[i + 0][j + 1]), std::cend(grid[i + 0][j + 1]));
                m_vertex_data.insert(std::end(m_vertex_data), std::cbegin(grid[i + 0][j + 0]), std::cend(grid[i + 0][j + 0]));
            }
        }

        glGenBuffers(1, &m_vertex_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * std::size(m_vertex_data), std::data(m_vertex_data), GL_STATIC_DRAW);

        auto uv = Mat_3(m_width, Mat_2(m_height, Mat_1(2, 0.0f)));

        for (auto i = 0ul; i < m_width; ++i) {
            for (auto j = 0ul; j < m_height; ++j) {
                uv[i][j][0] = static_cast<float>(i) / static_cast<float>(m_width);
                uv[i][j][1] = static_cast<float>(j) / static_cast<float>(m_height);
            }
        }

        for (auto i = 0ul; i < m_width - 1; ++i) {
            for (auto j = 0ul; j < m_height - 1; ++j) {
                m_uv_data.insert(std::end(m_uv_data), std::cbegin(uv[i + 0][j + 0]), std::cend(uv[i + 0][j + 0]));
                m_uv_data.insert(std::end(m_uv_data), std::cbegin(uv[i + 1][j + 0]), std::cend(uv[i + 1][j + 0]));
                m_uv_data.insert(std::end(m_uv_data), std::cbegin(uv[i + 1][j + 1]), std::cend(uv[i + 1][j + 1]));

                m_uv_data.insert(std::end(m_uv_data), std::cbegin(uv[i + 1][j + 1]), std::cend(uv[i + 1][j + 1]));
                m_uv_data.insert(std::end(m_uv_data), std::cbegin(uv[i + 0][j + 1]), std::cend(uv[i + 0][j + 1]));
                m_uv_data.insert(std::end(m_uv_data), std::cbegin(uv[i + 0][j + 0]), std::cend(uv[i + 0][j + 0]));
            }
        }

        glGenBuffers(1, &m_uv_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_uv_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * std::size(m_uv_data), std::data(m_uv_data), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_texture.gen_buffer();
    }

    auto render() -> void override {
        m_texture.bind();

        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, m_uv_vbo);
        glVertexAttribPointer(3, 2, GL_FLOAT, false, 0, nullptr);
        glEnableVertexAttribArray(3);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(m_vertex_count));

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
};

}

#endif //CPP_ENGINE_MODEL_VBO_GRID_HPP
