#include "SpotLight.h"
#include "GameObject.h"
#include "Transform.h"
#include "Helpers.h"

namespace Beans
{
  INIT_REFLECTION(SpotLight);

  SpotLight::SpotLight(GameObject * object) : Component(object)
  {
    transform_ = Owner->AddComponent<Transform>();
  }
  void SpotLight::SendLightsToShader(Shader * shader)
  {
    int i = 0;

    shader->SetUniformInt("spotLightCount", min(MAX_SPOT_LIGHTS, (int)GetList().size()));

    for (auto light : GetList())
    {
      std::string uniformName = "spotLights[";
      uniformName += std::to_string(i);
      uniformName += "].";

      shader->SetUniformVec3f((uniformName + "position").c_str(), light->transform_->Position);
      shader->SetUniformVec3f((uniformName + "direction").c_str(), light->transform_->GetForwardDirection());
      shader->SetUniformFloat((uniformName + "cutoff").c_str(), light->inner_cutoff);
      shader->SetUniformFloat((uniformName + "outerCutoff").c_str(), light->outer_cutoff);

      shader->SetUniformVec3f((uniformName + "ambient").c_str(), light->color * 0.05f);
      shader->SetUniformVec3f((uniformName + "diffuse").c_str(), light->color * 0.8f);
      shader->SetUniformVec3f((uniformName + "specular").c_str(), light->color * 1.0f);

      shader->SetUniformFloat((uniformName + "constant").c_str(), 1.0f);
      shader->SetUniformFloat((uniformName + "linear").c_str(), 1.0f / light->power * 0.022f);
      shader->SetUniformFloat((uniformName + "quadratic").c_str(), 1.0f / light->power * 0.0019f);

      glErrorCheck();

      ++i;

      if (i >= MAX_SPOT_LIGHTS) return;
    }
  }
}