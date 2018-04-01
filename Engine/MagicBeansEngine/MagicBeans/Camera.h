#pragma once

#include "Component.h"

#include "Transform.h"
#include <..\Includes\GLM\gtc\matrix_transform.hpp>

namespace Beans
{
    class Camera : public Component, public Bindable<Camera>
    {
    public:
        Camera(GameObject* owner);

        REFLECT_CLASS(Camera);

        vec2 size;

        void LookAt(const vec3& position);

        void UpdateMatrix();
        const mat4& GetMatrix();

        Property<Camera, vec3> GlobalUp;
        Property<Camera, vec3> LookDirection;

        const vec3& SetGlobalUp(const vec3&);
        vec3 GetGlobalUp() const;
        const vec3& SetLookDirection(const vec3&);
        vec3 GetLookDirection() const;

    private:
        mat4 matrix_;
        vec3 globalUp_;
        vec3 lookDirection_;
    };
}