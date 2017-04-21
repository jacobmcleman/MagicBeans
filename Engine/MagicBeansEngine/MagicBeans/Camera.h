#pragma once

#include "Component.h"

#include "Transform.h"
#include <GLM\gtc\matrix_transform.hpp>

namespace Beans
{
  class Camera : public Component
  {
  public:
    Camera(GameObject* owner);

    REFLECT_CLASS;

    vec2 size;

    void UpdateMatrix();
    const mat4& GetMatrix();
  private:
    mat4 matrix_;
  };
}