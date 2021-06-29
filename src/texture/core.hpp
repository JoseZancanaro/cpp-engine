#ifndef CPP_ENGINE_TEXTURE_CORE_HPP
#define CPP_ENGINE_TEXTURE_CORE_HPP

#include <array>
#include <cstdint>

#include <SFML/Graphics/Image.hpp>

#include "../gl.hpp"

namespace engine {

struct Texture {
    std::int32_t m_width;
    std::int32_t m_height;
    std::int64_t m_scaled_width;
    std::int64_t m_scaled_height;
    std::uint32_t m_id;
    std::vector<std::uint8_t> m_buffer;
    std::uint32_t m_vbo;

    Texture()
     : m_width(0),
       m_height(0),
       m_scaled_width(0),
       m_scaled_height(0),
       m_id(0),
       m_buffer()
    {}

    Texture(std::uint32_t id = 0)
    : m_width(0),
      m_height(0),
      m_scaled_width(0),
      m_scaled_height(0),
      m_id(id),
      m_buffer()
    {}

    template <class Path>
    Texture(Path && p, std::uint32_t id = 0)
     : m_id(id)
    {
        auto texture_img = sf::Image();
        texture_img.loadFromFile(p);

        auto [width, height] = texture_img.getSize();
        auto ptr = texture_img.getPixelsPtr();

        m_width = static_cast<int>(width);
        m_height = static_cast<int>(height);
        m_scaled_width = m_width;
        m_scaled_height = m_height;
        m_buffer = std::vector(ptr, ptr + width * height * 4);
    }

    auto gen_buffer() -> void {
        glGenTextures(1, &m_vbo);
        glBindTexture(GL_TEXTURE_2D, m_vbo);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, std::data(m_buffer));
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    auto bind() -> void {
        glActiveTexture(GL_TEXTURE0 + m_id);
        glBindTexture(GL_TEXTURE_2D, m_vbo);
    }
};

}

#endif //CPP_ENGINE_TEXTURE_CORE_HPP
