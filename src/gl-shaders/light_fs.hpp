#ifndef CPP_ENGINE_LIGHT_FS_HPP
#define CPP_ENGINE_LIGHT_FS_HPP

#include <string>

namespace engine::assets::gl_shaders {

auto light_fs_source = std::string(R"(

#version 330 core

in vec2 tex_vertex;
in vec3 normal;
in vec3 frag_pos;
in vec3 light_source;

uniform sampler2D tex;

out vec4 FragColor;

void main()
{
    vec4 pixel_texture = texture(tex, tex_vertex);

    vec3 L = normalize(light_source - frag_pos);
    vec3 E = normalize(-frag_pos);
    vec3 R = normalize(-reflect(L, normal));

    vec4 difuse_diff = clamp(vec4(1.0, 1.0, 1.0, 1.0) * max(dot(normal, L), 0.0), 0.0, 1.0);
    float d = (distance(frag_pos, light_source)) * 2;
    vec4 specular_diff = clamp(vec4(1.0, 1.0, 1.0, 1.0) * pow(max(dot(R, E), 0.0), 0.3 * 10), 0.0, 1.0);

    //FragColor = vec4(specular_diff.xyz + difuse_diff.xyz + max(vec3(1 - d, 1 - d, 1 - d), 0), 1.0) * pixel_texture;
    //FragColor = vec4(difuse_diff.xyz, 1.0);
    FragColor = vec4(normal * 0.5 + 0.5, 1.0);
}

)");
}

#endif //CPP_ENGINE_LIGHT_FS_HPP
