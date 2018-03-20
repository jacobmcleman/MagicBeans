#pragma once
#include "Component.h"
#include "AutoLister.h"
#include <..\Includes\GLM\glm.hpp>

#include "Shader.h"

using namespace glm;

#define MAX_SPOT_LIGHTS 8

namespace Beans
{
  class Transform;

  class SpotLight : public Component, protected Utilities::AutoLister<SpotLight>
  {
  public:
    REFLECT_CLASS;

    SpotLight(GameObject* object);

    float power;
    vec3 color;
    float inner_cutoff;
    float outer_cutoff;

    static void SendLightsToShader(Shader* shader);

  private:
    Transform* transform_;
  };
}