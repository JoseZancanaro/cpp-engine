#ifndef CPP_ENGINE_LIGHT_VS_HPP
#define CPP_ENGINE_LIGHT_VS_HPP

#include <string>

namespace engine::assets::gl_shaders {

auto light_vs_source = std::string(R"(

#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nor;
layout (location = 2) in vec2 uvc;
layout (location = 3) in vec3 tan;
layout (location = 4) in vec3 bit;

uniform mat4 projection;
uniform mat4 world;
uniform mat4 model;
uniform vec3 light;
uniform vec3 view;

out VS_OUT {
    vec3 frag_pos;
    vec2 tex_vertex;
    vec3 tan_light;
    vec3 tan_view;
    vec3 tan_frag_pos;
} vs_out;

void main()
{
    vs_out.frag_pos = vec3(vec4(pos, 1.0) * model);
    vs_out.tex_vertex = uvc;

    mat3 nor_mat = transpose(inverse(mat3(model)));
    vec3 T = normalize(nor_mat * tan);
    vec3 N = normalize(nor_mat * nor);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.tan_light = TBN * light;
    vs_out.tan_view = TBN * view;
    vs_out.tan_frag_pos = TBN * vs_out.frag_pos;

    gl_Position = projection * world * vec4(vs_out.frag_pos, 1.0);

}

)");

}

#endif //CPP_ENGINE_LIGHT_VS_HPP
