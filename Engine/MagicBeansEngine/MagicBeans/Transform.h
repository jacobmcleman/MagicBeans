#pragma once
#include "Component.h"
#define GLM_FORCE_CXX11
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
    Property<vec2> scale;

    const mat4& GetMatrix();

  private:
    bool needsUpdate_;;
    mat4 matrix_;
  };
}