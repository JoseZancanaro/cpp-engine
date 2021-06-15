#ifndef CPP_ENGINE_LOADER_HPP
#define CPP_ENGINE_LOADER_HPP

#include <fstream>
#include <memory>
#include <span>

#include <fmt/format.h>

#include <SFML/Graphics/Image.hpp>

#include "./header.hpp"

namespace engine::md2::io {

template <class Path>
auto read_texture(Path && p, std::uint32_t id = 0) -> md2::Texture {
    auto texture_img = sf::Image();
    texture_img.loadFromFile(p);

    auto [width, height] = texture_img.getSize();
    auto ptr = texture_img.getPixelsPtr();

    return {
        .width = static_cast<int>(width),
        .height = static_cast<int>(height),
        .id = id,
        .buffer = std::vector(ptr, ptr + width * height * 4)
    };
}

template <class Path, std::uint32_t I = md2::IDP2, std::uint32_t V = md2::VERSION>
auto read(Path && p) -> md2::Resource {
    auto header = md2::Header{};
    auto file = std::ifstream(p, std::ifstream::binary);

    file.read(reinterpret_cast<char*>(&header), sizeof(decltype(header)));

    if (header.ident == I && header.version == V) {
        file.seekg(0, std::ios::end);
        auto length = file.tellg();

        auto buffer = std::vector<char>(length);
        file.seekg(0, std::ios::beg);
        file.read(std::data(buffer), length);

        auto md2 = md2::Resource{};

        /* Copy metadata from reader */
        md2.num_points = header.num_points;
        md2.num_frames = header.num_frames;
        md2.frame_size = header.frame_size;
        md2.num_tex = header.num_tex;
        md2.num_mesh = header.num_mesh;

        /* Copy coordinates */
        md2.point.reserve(md2.num_points * md2.num_frames);

        for (auto i = 0ul; i < md2.num_frames; ++i) {
            auto frame = reinterpret_cast<Frame*>(&buffer[header.ofs_frames + i * md2.frame_size]);

            for (auto j = 0ul; j < md2.num_points; ++j) {
                md2.point.push_back(glm::vec3(
                    frame->scale[0] * frame->fp[j].vertex_data[0] + frame->translate[0],
                    frame->scale[2] * frame->fp[j].vertex_data[2] + frame->translate[2],
                    frame->scale[1] * frame->fp[j].vertex_data[1] + frame->translate[1]
                ));
            }
        }

        /* Copy texture coordinates */
        auto tex_vec = reinterpret_cast<Texture_Vec*>(&buffer[header.ofs_tex]);
        md2.tex.reserve(md2.num_tex);

        for (auto i = 0ul; i < md2.num_tex; ++i) {
            md2.tex.push_back(glm::vec2(
                tex_vec[i].s / static_cast<float>(header.skin_width),
                tex_vec[i].t / static_cast<float>(header.skin_height)
            ));
        }

        /* Copy mesh data */
        auto mesh = reinterpret_cast<Mesh*>(&buffer[header.ofs_mesh]);
        md2.mesh.reserve(md2.num_mesh);

        for (auto i = 0ul; i < md2.num_mesh; ++i) {
            md2.mesh.push_back(mesh[i]);
        }

        /* Copy gl commands */
        auto gl_cmd = reinterpret_cast<std::int32_t*>(&buffer[header.ofs_gl_cmds]);
        md2.gl_cmds.reserve(header.num_gl_cmds);

        for (auto i = 0ul; i < header.num_gl_cmds; ++i, ++gl_cmd) {
            md2.gl_cmds.push_back(*gl_cmd);
        }

        return md2;
    }

    return {};
}

template <class P1, class P2, std::uint32_t I = md2::IDP2, std::uint32_t V = md2::VERSION>
auto read(P1 && data, P2 && texture) -> md2::Resource {
    auto resource = read<P1, I, V>(data);
    resource.tex_data = read_texture<P2>(texture);

    return resource;
}

}

#endif //CPP_ENGINE_LOADER_HPP
