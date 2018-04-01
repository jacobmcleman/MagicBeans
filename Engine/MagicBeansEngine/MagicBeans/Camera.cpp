#include "Camera.h"

#include "GameObject.h"
#include "MathFunctions.h"

#define PERSPECTIVE

namespace Beans
{
    INIT_REFLECTION(Camera);

    Camera::Camera(GameObject * owner) : Component(owner), size(30, 30),
        GlobalUp(this, "GlobalUp", &Camera::GlobalUp, &Camera::GetGlobalUp, &Camera::SetGlobalUp, vec3(0, 0, 1)),
        LookDirection(this, "LookDirection", &Camera::LookDirection, &Camera::GetLookDirection, &Camera::SetLookDirection, vec3(0, 1, 0))
    {
    }

    void Camera::LookAt(const vec3 & position)
    {
        (void)position;
        //TODO actually implement this function
        //LookDirection = (position - Owner->GetComponent<Transform>()->position.Data());
    }

    void Camera::UpdateMatrix()
    {
#ifndef PERSPECTIVE
        float top, bottom, left, right;
        top = ((vec3)Owner->GetComponent<Transform>()->position).y + (size.y / 2);
        bottom = ((vec3)Owner->GetComponent<Transform>()->position).y - (size.y / 2);
        left = ((vec3)Owner->GetComponent<Transform>()->position).x - (size.x / 2);
        right = ((vec3)Owner->GetComponent<Transform>()->position).x + (size.x / 2);
        matrix_ = glm::ortho(left, right, bottom, top);
#else
        vec3 lookDir = Owner->GetComponent<Transform>()->GetRotationQuaternion() * vec3(0, 1, 0);
        mat4 view =
            glm::lookAt(Owner->GetComponent<Transform>()->Position.Get(), //Eye position
                Owner->GetComponent<Transform>()->Position + lookDir, //Camera center
                GlobalUp.Get()); //Up vector

        mat4 projection = glm::perspective(45.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);

        matrix_ = projection * view;
#endif
    }
    const mat4& Camera::GetMatrix()
    {
        return matrix_;
    }
    const vec3& Camera::SetGlobalUp(const vec3& input)
    {
        globalUp_ = normalize(input);
        return globalUp_;
    }
    vec3 Camera::GetGlobalUp() const
    {
        return  globalUp_;
    }
    const vec3& Camera::SetLookDirection(const vec3& input)
    {
        lookDirection_ = normalize(input);
        return lookDirection_;
    }
    vec3 Camera::GetLookDirection() const
    {
        return Owner->GetComponent<Transform>()->GetRotationQuaternion() * vec3(0, 1, 0);
    }
}

