#include "Camera.h"

#include "GameObject.h"

#define PERSPECTIVE

namespace Beans
{
  INIT_REFLECTION(Camera);

  Camera::Camera(GameObject * owner) : Component(owner), size(30, 30)
  {
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
    mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    mat4 view;
    view = glm::translate(view, -1.0f * (Owner->GetComponent<Transform>()->position.Data()));
    view = glm::rotate(view, radians(-90.0f), vec3(1, 0, 0));
    matrix_ = projection * view;
#endif
  }
  const mat4& Camera::GetMatrix()
  {
    return matrix_;
  }
}

