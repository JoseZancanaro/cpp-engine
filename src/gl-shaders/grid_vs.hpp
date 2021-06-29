#ifndef CPP_ENGINE_GRID_VS_HPP
#define CPP_ENGINE_GRID_VS_HPP

#include <string>

namespace engine::assets::gl_shaders {

auto grid_vs_source = std::string(R"(

#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 3) in vec2 t_pos;

uniform mat4 view;

uniform float timer;
uniform float intensity;
uniform vec2 center;

out vec2 tex_vertex;

void main()
{
    vec4 pos = vec4(a_pos, 1.0);
    vec4 beg = vec4(center, pos.z, 1.0) * view;

    float dx = pos.x - beg.x;
    float dy = pos.y - beg.y;
    float d = sqrt(dx * dx + dy * dy);
    pos.z = (sin(-timer * 7 + d * 15) * (0.5 / (1 + (d * 5) * (d * 5)))) * intensity;

    gl_Position = pos * view;
    tex_vertex = vec2(t_pos.y, 1 - t_pos.x);
}

)");

}

#endif //CPP_ENGINE_GRID_VS_HPP