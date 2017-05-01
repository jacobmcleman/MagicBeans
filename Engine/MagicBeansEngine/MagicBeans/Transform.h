#pragma once
#include "Component.h"
#include <GLM\glm.hpp>


using namespace glm;

namespace Beans
{
  class Transform : public Component
  {
  public:
    Transform(GameObject* owner);

    REFLECT_CLASS;

    Property<vec3> position;
    Property<float> rotation;
    Property<vec3> scale;

    const mat4& GetMatrix();

  private:
    bool needsUpdate_;;
    mat4 matrix_;
  };
}