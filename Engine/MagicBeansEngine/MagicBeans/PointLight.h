#pragma once
#include "Component.h"
#include "AutoLister.h"
#include "../Includes/GLM/glm.hpp"

#include "Shader.h"

using namespace glm;

#define MAX_POINT_LIGHTS 16

namespace Beans
{
  class Transform;

  class PointLight : public Component, protected Utilities::AutoLister<PointLight>
  {
  public:
    REFLECT_CLASS;

    PointLight(GameObject* owner);
    virtual ~PointLight();

    float power;
    vec3 color;

    static void SendLightsToShader(Shader* shader);

  private:
    Transform* transform_;
  };
}