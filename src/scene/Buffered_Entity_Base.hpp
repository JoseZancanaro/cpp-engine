#ifndef CPP_ENGINE_BUFFERED_ENTITY_BASE_HPP
#define CPP_ENGINE_BUFFERED_ENTITY_BASE_HPP

#include "./Entity_Base.hpp"

namespace engine {

class Buffered_Entity_Base : public Entity_Base {
public:
    virtual auto load() -> void = 0;
    ~Buffered_Entity_Base() override = default;
};

}

#endif //CPP_ENGINE_BUFFERED_ENTITY_BASE_HPP
