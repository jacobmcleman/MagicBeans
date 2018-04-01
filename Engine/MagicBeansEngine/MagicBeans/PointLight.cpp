#include "PointLight.h"
#include "GameObject.h"
#include "Transform.h"
#include "Helpers.h"

namespace Beans
{
  INIT_REFLECTION(PointLight);

  PointLight::PointLight(GameObject * owner) : Component(owner), power(1), color(1, 1, 1)
  {
    transform_ = Owner->AddComponent<Transform>();
  }

  PointLight::~PointLight()
  {
  }

  void PointLight::SendLightsToShader(Shader * shader)
  {
    int i = 0;

    shader->SetUniformInt("pointLightCount", min(MAX_POINT_LIGHTS, (int)GetList().size()));

    for (auto light : GetList())
    {
      std::string uniformName = "pointLights[";
      uniformName += std::to_string(i);
      uniformName += "].";

      shader->SetUniformVec3f((uniformName + "position").c_str(), light->transform_->Position);

      shader->SetUniformVec3f((uniformName + "ambient").c_str(), light->color * 0.2f);
      shader->SetUniformVec3f((uniformName + "diffuse").c_str(), light->color * 0.8f);
      shader->SetUniformVec3f((uniformName + "specular").c_str(), light->color * 1.0f);

      shader->SetUniformFloat((uniformName + "constant").c_str(), 1.0f);
      shader->SetUniformFloat((uniformName + "linear").c_str(), 1.0f / light->power * 0.22f);
      shader->SetUniformFloat((uniformName + "quadratic").c_str(), 1.0f / light->power * 0.019f);
			
      glErrorCheck();

      ++i;

      if (i >= MAX_POINT_LIGHTS) return;
    }
  }
}