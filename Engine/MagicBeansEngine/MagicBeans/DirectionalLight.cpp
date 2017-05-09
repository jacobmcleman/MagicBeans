#include "DirectionalLight.h"
#include "GameObject.h"
#include "Transform.h"
#include "Helpers.h"

namespace Beans
{
  INIT_REFLECTION(DirectionalLight);

  DirectionalLight::DirectionalLight(GameObject * owner) : Component(owner)
  {
    transform_ = Owner->AddComponent<Transform>();
  }

  DirectionalLight::~DirectionalLight()
  {
  }

  void DirectionalLight::SendLightsToShader(Shader * shader)
  {
    int i = 0;

    shader->SetUniformInt("directionalLightCount", min(MAX_DIRECTIONAL_LIGHTS, (int)GetList().size()));
  
    for (auto light : GetList())
    {
      std::string uniformName = "directionalLights[";
      uniformName += std::to_string(i);
      uniformName += "].";

      shader->SetUniformVec3f((uniformName + "direction").c_str(), light->transform_->GetForwardDirection());

      shader->SetUniformVec3f((uniformName + "color").c_str(), light->color);

      glErrorCheck();

      ++i;

      if (i >= MAX_DIRECTIONAL_LIGHTS) return;
    }
  }

}
