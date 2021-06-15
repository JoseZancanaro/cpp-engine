#ifndef CPP_ENGINE_HEADER_HPP
#define CPP_ENGINE_HEADER_HPP

#include <array>
#include <cstdint>
#include <string>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace engine::md2 {

constexpr auto IDP2 = std::uint32_t(('2' << 24) + ('P' << 16) + ('D' << 8) + 'I');
constexpr auto VERSION = std::uint32_t(8);

struct Header {
    int ident;
    int version;
    int skin_width;
    int skin_height;
    int frame_size;
    int num_skins;
    int num_points;
    int num_tex;
    int num_mesh;
    int num_gl_cmds;
    int num_frames;
    int ofs_skins;
    int ofs_tex;
    int ofs_mesh;
    int ofs_frames;
    int ofs_gl_cmds;
    int ofs_end;
};

struct Texture_Vec {
    std::int16_t s;
    std::int16_t t;
};

struct Frame_Point {
    std::array<std::uint8_t, 3> vertex_data;
    std::uint8_t normal_index;
};

struct Frame {
    std::array<float, 3> scale;
    std::array<float, 3> translate;
    std::array<char, 16> name;
    std::array<Frame_Point, 1> fp;
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
    std::vector<std::uint8_t> buffer;
};

struct Resource {
    int num_frames;
    int num_points;
    int num_mesh;
    int num_tex;
    int frame_size;
    int tex_width;
    int tex_height;
    std::vector<Mesh> mesh;
    std::vector<glm::vec2> tex;
    std::vector<glm::vec3> point;
    Texture tex_data;
    std::vector<int> gl_cmds;
};

struct Sprint_Key {
    std::uint32_t first_frame;
    std::uint32_t last_frame;
    std::uint32_t fps;
};

struct Sprint_State {
    Sprint_Key current_sprint;
    std::uint32_t current_frame;
    std::uint32_t next_frame;
    float current_time;
    float old_time;
    float lerp;
};

/* Quake2 sprints */
constexpr auto Model_Sprints = std::array<Sprint_Key, 21> {{
    {   0,  39,  9 }, /* STAND */
    {  40,  45, 10 }, /* RUN */
    {  46,  53, 10 }, /* ATTACK */
    {  54,  57,  7 }, /* PAIN_A */
    {  58,  61,  7 }, /* PAIN_B */
    {  62,  65,  7 }, /* PAIN_C */
    {  66,  71,  7 }, /* JUMP */
    {  72,  83,  7 }, /* FLIP */
    {  84,  94,  7 }, /* SALUTE */
    {  95, 111, 10 }, /* FALLBACK */
    { 112, 122,  7 }, /* WAVE */
    { 123, 134,  6 }, /* POINT */
    { 135, 153, 10 }, /* CROUCH_STAND */
    { 154, 159,  7 }, /* CROUCH_WALK */
    { 160, 168, 10 }, /* CROUCH_ATTACK */
    { 169, 172,  7 }, /* CROUCH_PAIN */
    { 173, 177,  5 }, /* CROUCH_DEATH */
    { 178, 183,  7 }, /* DEATH_FALLBACK */
    { 184, 189,  7 }, /* DEATH_FALLFORWARD */
    { 190, 197,  7 }, /* DEATH_FALLBACKSLOW */
    { 198, 198,  5 }, /* BOOM */
}};

}

#endif //CPP_ENGINE_HEADER_HPP
