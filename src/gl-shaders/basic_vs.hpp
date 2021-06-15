#ifndef CPP_ENGINE_BASIC_VS_HPP
#define CPP_ENGINE_BASIC_VS_HPP

#include <string>

namespace engine::assets::gl_shaders {

auto basic_vs_source = std::string(R"(

#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 3) in vec3 n_pos;
layout (location = 2) in vec3 a_nor;
layout (location = 4) in vec3 n_nor;
layout (location = 1) in vec2 t_pos;

uniform mat4 transform;
uniform float lerp;

out vec2 tex_vertex;
out vec3 normal;
out vec3 frag_pos;

void main()
{
    vec3 lerp_vertex = a_pos;
    if (lerp >= 0.0f) lerp_vertex += lerp * (n_pos - a_pos);

    vec3 lerp_normal = a_nor;
    if (lerp >= 0.0f) lerp_normal += lerp * (n_nor - a_nor);

    gl_Position = transform * vec4(lerp_vertex, 1.0);
    tex_vertex = t_pos;
    normal = lerp_normal;
    frag_pos = vec3(transform * vec4(lerp_vertex, 1.0));
}

)");

}

#endif //CPP_ENGINE_BASIC_VS_HPP
