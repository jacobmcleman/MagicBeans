#pragma once
#include "Component.h"
#include "Bindable.h"
#include <..\includes\GLM\glm.hpp>
#include <..\includes\GLM\gtc\quaternion.hpp>


using namespace glm;

namespace Beans
{
    class Transform : public Component, public Bindable<Transform>
    {
    public:
        Transform(GameObject* owner);

        REFLECT_CLASS(Transform);

        Property<Transform, vec3> Position;
        Property<Transform, vec3> Rotation;
        Property<Transform, vec3> Scale;

        const mat4& GetMatrix() const;

        void RotateBy(vec3 eulerAngles);

        vec3 GetForwardDirection();

        quat GetRotationQuaternion() const;

        vec3 GetPosition() const;
        const vec3& SetPosition(const vec3& aPosition);
        vec3 GetRotation() const;
        const vec3& SetRotation(const vec3& aRotation);
        vec3 GetScale() const;
        const vec3& SetScale(const vec3& aScale);

    private:
        mutable bool needsUpdate_;;
        mutable mat4 matrix_;
        quat rotationQuat_;
        vec3 position_;
        vec3 rotationEuler_;
        vec3 scale_;
    };
}