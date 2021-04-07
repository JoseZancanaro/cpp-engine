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

namespace engine::io::details {

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
    while (count < n
            && (stream >> buffer).fail() == false) {
        *(it++) = buffer;
        ++count;
    }

    return count;
}

template <class T = double>
auto parse_vertex_data(std::string_view view, std::size_t count = 4) -> std::vector<T> {
    if (auto index = find_first_numeric(view); index != std::end(view)) {
        view.remove_prefix(std::distance(std::begin(view), index));

        auto data = std::vector<T>{};
        read_stream_n<T>(std::istringstream(std::string(view)),
                            std::back_inserter(data), count);

        return data;
    }

    return {};
}

template <class Value_Type, class Stream, class It>
auto parse_face_triple(Stream && stream, It it) -> std::size_t {
    auto count = 0ul;
    auto buffer = Value_Type{};

    auto peek_slash = [](auto & stream) -> bool {
        auto peek = stream.peek();
        if (peek == '/' && stream.get()) {
            stream.clear();
            return true;
        }
        return false;
    };

    /* Parse V index */
    if ((stream >> buffer).fail() == false) /* [ 1 ] */ {
        *(it++) = buffer;
        ++count;

        if (peek_slash(stream)) /* [ 1/ ] */ {
            /* Parse VT index */
            if ((stream >> buffer).fail() == false) /* [ 1/1 ]  */ {
                *(it++) = buffer;
                ++count;
            }
            else /* [ 1/. ] */ {
                stream.clear();
                ++it;
            }

            if (peek_slash(stream)) /* [ 1/./ ] */ {
                /* Parse VN index */
                if ((stream >> buffer).fail() == false) /* [ 1/./1 ]  */ {
                    *(it++) = buffer;
                    ++count;
                }
            }
        }
    }

    return count;
}

template <class T = double>
auto parse_face_data(std::string_view view) -> typename Solid<T>::Face_Indexer {
    if (auto index = find_first_numeric(view); index != std::end(view)) {
        view.remove_prefix(std::distance(std::begin(view), index));

        auto face_idx = typename Solid<T>::Face_Indexer{};
        auto stream = std::istringstream(std::string(view));

        while (stream.fail() == false && stream.eof() == false) {
            auto data = std::array<std::size_t, 3>{};
            auto read = parse_face_triple<int>(stream, std::begin(data));

            if (read > 0) {
                face_idx.indexes.push_back(data.front());
            }
        }

        return face_idx;
    }

    return {};
}

template <class T = double>
auto parse_tag_vertex(std::string_view view) -> util::Result<Vector_3D<T>, std::string_view> {
    auto vertex = parse_vertex_data<T>(view);

    if (std::size(vertex) >= 3) {
        return { .data = { vertex[0], vertex[1], vertex[2] } };
    }

    return { .err = "Malformed vertex data", .is_err = true };
}

template <class T = double>
auto read_tag_vertex_normal(std::string_view view) {}

template <class T>
auto read_tag_face(std::string_view view) -> util::Result<typename Solid<T>::Face_Indexer, std::string_view> {
    auto face_indexer = parse_face_data(view);

    if (std::size(face_indexer.indexes) > 2) {
        return { .data = std::move(face_indexer) };
    }

    return { .err = "Malformed face data", .is_err = true };
}

template <class T = double>
auto parse_wv_obj(std::ifstream & input_file) -> Solid<T> {
    // 3d model data
    auto solid = Solid<T>{};

    // context
    auto buffer = std::string{};
    auto count = 0ul;

    while (std::getline(input_file, buffer).good()) {
        auto view = trim_view(std::string_view{buffer});

        if (std::size(view) > 2) { /* minimum size for comparison */
            if (view[0] == '#') /* comment */ {}
            else if (view[0] == 'v' && view[1] == ' ') /* vertex */ {
                if (auto result = parse_tag_vertex<T>(view); result.ok()) {
                    solid.vertex.push_back(std::move(result.data));
                }
                else {
                    fmt::print("Err({} on LINE {})\n", result.err, count);
                }
            }
            else if (view[0] == 'v' && view[1] == 'n') /* vertex normal */ {}
            else if (view[0] == 'f' && view[1] == ' ') /* face */ {
                if (auto result = read_tag_face<T>(view); result.ok()) {
                    solid.faces.push_back(std::move(result.data));
                }
                else {
                    fmt::print("Err({} on LINE {})\n", result.err, count);
                }
            }
        }

        ++count;
    }

    return solid;
}

} // namespace engine::io::details

namespace engine::io {

template<class Path, class D = double>
auto read_wavefront(Path && p) -> Solid<D> {
    auto input_file = std::ifstream{p};

    if (input_file) {
        return details::parse_wv_obj(input_file);
    }

    return {};
}

} // namespace engine::io

#endif //CPP_ENGINE_OBJ_READER_HPP
