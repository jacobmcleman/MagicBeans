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

    void LookAt(const vec3& position);

    void UpdateMatrix();
    const mat4& GetMatrix();

    Property<vec3> GlobalUp;
    Property<vec3> LookDirection;

  private:
    mat4 matrix_;
  };
}