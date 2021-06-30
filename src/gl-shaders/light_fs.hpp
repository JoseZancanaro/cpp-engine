#ifndef CPP_ENGINE_LIGHT_FS_HPP
#define CPP_ENGINE_LIGHT_FS_HPP

#include <string>

namespace engine::assets::gl_shaders {

auto light_fs_source = std::string(R"(

#version 330 core

in VS_OUT {
    vec3 frag_pos;
    vec2 tex_vertex;
    vec3 tan_light;
    vec3 tan_view;
    vec3 tan_frag_pos;
} fs_in;

uniform sampler2D diffuse_map;
uniform sampler2D normal_map;

uniform vec3 light;

out vec4 frag_color;

void main()
{
    vec3 normal_texture = normalize(texture(normal_map, fs_in.tex_vertex).rgb * 2 - 1);

    vec3 pixel_texture = texture(diffuse_map, fs_in.tex_vertex).rgb;

    vec3 ambient = 0.1 * pixel_texture;

    // Diffuse Light
    vec3 light_dir = normalize(fs_in.tan_light - fs_in.tan_frag_pos);
    float diff = max(dot(light_dir, normal_texture), 0.0);
    vec3 diffuse = diff * pixel_texture;

    // Specular Light
    vec3 view_dir = normalize(fs_in.tan_view - fs_in.tan_frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal_texture);
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal_texture, halfway_dir), 0.0), 32.0);
    vec3 specular = vec3(0.2) * spec;

    frag_color = vec4(ambient + diffuse + specular, 1.0);
}

)");
}

#endif //CPP_ENGINE_LIGHT_FS_HPP
