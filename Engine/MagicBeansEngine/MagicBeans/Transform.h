#pragma once
#include "Component.h"
#include <GLM\glm.hpp>
#include <GLM\gtc\quaternion.hpp>

using namespace glm;

namespace Beans
{
  class Transform : public Component
  {
  public:
    Transform(GameObject* owner);

    REFLECT_CLASS;

    Property<vec3> position;
    Property<vec3> rotation;
    Property<vec3> scale;

    const mat4& GetMatrix();

    void RotateBy(vec3 eulerAngles);

    quat GetRotationQuaternion() const;

  private:
    bool needsUpdate_;;
    mat4 matrix_;
    quat rotation_;
  };
}