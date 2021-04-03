#ifndef CPP_ENGINE_OBJ_READER_HPP
#define CPP_ENGINE_OBJ_READER_HPP

#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <filesystem>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include "../geometry/core.hpp"
#include "../utility/result.hpp"

namespace engine::io {

namespace details {

auto trim_view(std::string_view view) -> std::string_view {
    view.remove_prefix(std::min(view.find_first_not_of(' '), std::size(view)));
    view.remove_suffix(std::min(std::size(view) - view.find_last_not_of(' ') - 1, std::size(view)));

    return view;
}

auto find_first_numeric(std::string_view view) {
    return std::find_if(std::begin(view), std::end(view), [](auto c){
        return c >= '0' && c <= '9' || c == '-';
    });
}

template <class Value_Type, class Stream, class It>
auto read_stream_n(Stream && stream, It it, std::size_t n) -> std::size_t {
    auto count = 0ul;
    auto buffer = Value_Type{};

    /* Read stream n successful times */
    while (count < n && stream.good()) {
        stream >> buffer;

        if (stream.fail() == false) {
            *(it++) = buffer;
            ++count;
        }
    }

    return count;
}

} // namespace engine::io::details

template <class T = double>
auto parse_vertex_data(std::string_view view, std::size_t count = 4) -> std::vector<T> {
    if (auto index = details::find_first_numeric(view); index != std::end(view)) {
        view.remove_prefix(std::distance(std::begin(view), index));

        auto data = std::vector<T>{};
        details::read_stream_n<T>(std::istringstream(std::string(view)),
                                    std::back_inserter(data), count);

        return data;
    }

    return {};
}

auto parse_tag_vertex(std::string_view view) -> util::Result<Vector_3D<double>, std::string_view> {
    auto vertex = parse_vertex_data(view);

    if (std::size(vertex) >= 3) {
        return { .data = { vertex[0], vertex[1], vertex[2] } };
    }

    return { .err = "Malformed vertex data", .is_err = true };
}

auto read_tag_vertex_normal(std::string_view view) {

}

auto read_tag_face() {

}

auto parse_wv_obj(std::ifstream & input_file) {
    // 3d model data
    auto vertex = std::vector<Vector_3D<double>>{};

    // context
    auto buffer = std::string{};
    auto count = 0ul;

    while (std::getline(input_file, buffer).good()) {
        //fmt::print("{} [Size:{}]\n", buffer, std::size(buffer));
        auto view = details::trim_view(std::string_view{buffer});

        if (std::size(view) > 2) { /* minimum size for comparison */
            if (view[0] == '#') /* comment */ {}
            else if (view[0] == 'v' && view[1] == ' ') /* vertex */ {
                if (auto result = parse_tag_vertex(view); result.ok()) {
                    vertex.push_back(std::move(result.data));
                }
                else {
                    fmt::print("Err({} on LINE {})\n", result.err, count);
                }
            }
            else if (view[0] == 'v' && view[1] == 'n') /* vertex normal */ {

            }
            else if (view[0] == 'f' && view[1] == ' ') /* face */ {

            }
        }

        ++count;
    }
}

template<class Path>
auto read(Path &&p) -> void /* Model_3D? */ {
    auto input_file = std::ifstream{p};

    fmt::print("std::fs::exists({}) -> {}\n", p, std::filesystem::exists(p));

    if (input_file) {
        fmt::print("Ok({})\n", p);
        parse_wv_obj(input_file);
    }
    else {
        fmt::print("Err({})\n", p);
    }
}

} // namespace engine::io

#endif //CPP_ENGINE_OBJ_READER_HPP
