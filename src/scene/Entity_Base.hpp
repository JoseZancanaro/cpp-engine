#ifndef CPP_ENGINE_ENTITY_BASE_HPP
#define CPP_ENGINE_ENTITY_BASE_HPP

namespace engine {

class Entity_Base {
public:
    virtual auto draw() -> void {};
    virtual auto update(float seconds) -> void {};
    virtual ~Entity_Base() = default;
};

}

#endif //CPP_ENGINE_ENTITY_BASE_HPP
