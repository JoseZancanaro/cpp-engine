#ifndef CPP_ENGINE_ENTITY_OWNER_HPP
#define CPP_ENGINE_ENTITY_OWNER_HPP

#include <memory>
#include <numbers>
#include <utility>

#include <fmt/format.h>

#include "../geometry/core.hpp"

#include "../gl.hpp"
#include "./Buffered_Entity_Base.hpp"

namespace engine {

class Entity_Owner : public Entity_Base {
public:
    using Entity_Ptr = std::shared_ptr<Buffered_Entity_Base>;

    Entity_Ptr m_model;
    Vector_3Df m_position;
    float m_angle;

    explicit Entity_Owner(Entity_Ptr model, Vector_3Df position) :
     m_model(std::move(model)),
     m_position(std::move(position)),
     m_angle()
    {}

    auto draw() -> void override {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glLoadIdentity();
        glTranslatef(m_position.x, m_position.y, m_position.z);
        glScalef(0.005f, 0.005f, 0.005f);
        glRotatef(m_angle, 0.0f, 1.0f, 0.0f);
        glColor3f(1.0f, 0.0f, 0.0f);

        m_model->draw();

        glPopMatrix();
    }

    auto update(float seconds) -> void override {
        m_angle += 15.0f * seconds * std::numbers::pi_v<float> / 180.0f * 60.0f;
    }

    ~Entity_Owner() override = default;
};

}



#endif //CPP_ENGINE_ENTITY_OWNER_HPP
