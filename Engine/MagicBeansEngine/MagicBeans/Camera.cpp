#include "Camera.h"

#include "GameObject.h"
#include "MathFunctions.h"

#define PERSPECTIVE

namespace Beans
{
  INIT_REFLECTION(Camera);

  Camera::Camera(GameObject * owner) : Component(owner), size(30, 30), 
    LookDirection(vec3(0, 1, 0), [&](const vec3& input)->vec3 { return normalize(input); }),
    GlobalUp(vec3(0, 0, 1), [&](const vec3& input)->vec3 { return normalize(input); })
  {
  }

  void Camera::LookAt(const vec3 & position)
  {
    LookDirection = (position - Owner->GetComponent<Transform>()->position.Data());
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
    mat4 view = 
      glm::lookAt(Owner->GetComponent<Transform>()->position.Data(), //Eye position
                  Owner->GetComponent<Transform>()->position + LookDirection, //Camera center
                  GlobalUp.Data()); //Up vector

    mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    matrix_ = projection * view;
#endif
  }
  const mat4& Camera::GetMatrix()
  {
    return matrix_;
  }
}

