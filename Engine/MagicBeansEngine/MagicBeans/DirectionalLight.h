#pragma once
#include "Component.h"
#include "AutoLister.h"
#include "GLM\glm.hpp"

#include "Shader.h"

using namespace glm;

#define MAX_DIRECTIONAL_LIGHTS 4

namespace Beans
{
  class Transform;

  class DirectionalLight : public Component, protected Utilities::AutoLister<DirectionalLight>
  {
  public:
    REFLECT_CLASS;

    DirectionalLight(GameObject* owner);
    virtual ~DirectionalLight();

    float power;
    vec3 color;

    static void SendLightsToShader(Shader* shader);

  private:
    Transform* transform_;
  };
}