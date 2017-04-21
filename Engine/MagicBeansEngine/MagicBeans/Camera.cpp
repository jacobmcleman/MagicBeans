#include "Camera.h"

#include "GameObject.h"

namespace Beans
{
  INIT_REFLECTION(Camera);

  Camera::Camera(GameObject * owner) : Component(owner)
  {
  }

  void Camera::UpdateMatrix()
  {
    float top, bottom, left, right;
    top = ((vec3)Owner->GetComponent<Transform>()->position).y + (size.y / 2);
    bottom = ((vec3)Owner->GetComponent<Transform>()->position).y - (size.y / 2);
    left = ((vec3)Owner->GetComponent<Transform>()->position).x - (size.x / 2);
    right = ((vec3)Owner->GetComponent<Transform>()->position).x + (size.x / 2);
    matrix_ = glm::ortho(left, right, bottom, top);
  }
  const mat4& Camera::GetMatrix()
  {
    return matrix_;
  }
}

