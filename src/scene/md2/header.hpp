#ifndef CPP_ENGINE_MD2_HEADER_HPP
#define CPP_ENGINE_MD2_HEADER_HPP

#include <array>
#include <cstdint>
#include <string>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace engine::md2 {

struct Md2_Header {
    int ident;
    int version;
    int skin_width;
    int skin_height;
    int frame_size;
    int num_skins;
    int num_vecs;
    int num_st;
    int num_tris;
    int num_glcmds;
    int num_frames;
    int ofs_skins;
    int ofs_st;
    int ofs_tris;
    int ofs_frames;
    int ofs_glcmds;
    int ofs_end;
};

struct Texture_Vec {
    std::int16_t s;
    std::int16_t t;
};

struct Frame_Point {
    std::array<std::uint8_t, 3> vertex_data;
};

struct Frame {
    std::array<float, 3> scale;
    std::array<float, 3> translate;
    std::array<char, 16> name;
    Frame_Point fp;
};

struct Mesh {
    std::array<std::uint16_t, 3> vec_index;
    std::array<std::uint16_t, 3> tex_index;
};

struct Texture {
    int width;
    int height;
    std::int64_t scaled_width;
    std::int64_t scaled_height;
    std::uint32_t id;
    std::string data;
    std::string pallete;
};

struct Animation {
    int num_frames;
    int num_points;
    int num_mesh;
    int num_tex;
    int frame_size;
    int tex_width;
    int tex_height;
    int current_frame;
    int next_frame;
    float lerp;
    std::vector<Mesh> mesh_index;
    std::vector<glm::vec2> tex;
    std::vector<glm::vec3> point_list;
    std::vector<Texture> tex_data;
    std::vector<int> gl_cmds;
};
}

#endif //CPP_ENGINE_MD2_HEADER_HPP
