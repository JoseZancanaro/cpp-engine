#ifndef CPP_ENGINE_SHADER_CORE_HPP
#define CPP_ENGINE_SHADER_CORE_HPP

#include <array>
#include <string>

#include <fmt/core.h>

#include "../../gl.hpp"

namespace engine::shader {

auto createShader(std::uint32_t type, std::string const& str) -> std::uint32_t {
    auto source = str.c_str();
    auto id = glCreateShader(type);

    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    auto status = std::int32_t{};
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        auto error_buffer = std::array<char, 128>{};
        glGetShaderInfoLog(id, std::size(error_buffer), nullptr, std::data(error_buffer));
        fmt::print("Shader [{}] compilation error: {}\n", type, error_buffer._Elems);
    }

    return id;
}

auto createProgram(std::string const& vertex_shader, std::string const& fragment_shader) -> std::uint32_t {
    auto id = glCreateProgram();

    auto vs = createShader(GL_VERTEX_SHADER, vertex_shader);
    auto fs = createShader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(id, vs);
    glAttachShader(id, fs);
    glLinkProgram(id);

    glValidateProgram(id);

    return id;
}

} // engine::shader

#endif //CPP_ENGINE_SHADER_CORE_HPP
