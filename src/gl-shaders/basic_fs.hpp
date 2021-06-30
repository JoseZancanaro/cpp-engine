#ifndef CPP_ENGINE_BASIC_FS_HPP
#define CPP_ENGINE_BASIC_FS_HPP

#include <string>

namespace engine::assets::gl_shaders {

auto basic_fs_source = std::string(R"(

#version 330 core

in vec2 tex_vertex;
in vec3 normal;
in vec3 frag_pos;

uniform sampler2D tex;

out vec4 FragColor;

void main()
{
    vec4 pixel_texture = texture(tex, tex_vertex);
    FragColor = pixel_texture;
}

)");
}

#endif //CPP_ENGINE_BASIC_FS_HPP
