#ifndef CPP_ENGINE_RESULT_HPP
#define CPP_ENGINE_RESULT_HPP

#include <utility>

namespace engine::util {

template <class Res>
struct Ok {
    Res data;
};

template <class Error>
struct Err {
    Error data;
};

template <class Res, class Err>
struct Result {
    Res data;
    Err err;
    bool is_err = {};

    auto ok() const noexcept -> bool {
        return is_err == false;
    }
};

}

#endif //CPP_ENGINE_RESULT_HPP
