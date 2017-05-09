#include "DirectionalLight.h"
#include "GameObject.h"
#include "Transform.h"
#include "Helpers.h"
#include "MagicBeansEngine.h"
#include "CubeMesh.h"

#include <GLM\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Beans
{
  INIT_REFLECTION(DirectionalLight);

  MagicBeansEngine* DirectionalLight::engine_ = nullptr;
  Shader* DirectionalLight::shadowMapper_ = nullptr;

  DirectionalLight::DirectionalLight(GameObject * owner) : Component(owner)
  {
    transform_ = Owner->AddComponent<Transform>();

    glGenFramebuffers(1, &depthMapFBO_);
    glGenTextures(1, &depthMap_);

    glBindTexture(GL_TEXTURE_2D, depthMap_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap_, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

      shader->SetUniformMat4f(("lightSpaceMatrix[" + std::to_string(i) + "]").c_str(), light->lightSpaceTransform_);

      glErrorCheck();

      //Textures 0-MAX_DIRECTIONAL_LIGHTS will be used for depth cubemaps
      GLint tex = shader->getUniformLoc((uniformName + "depthMap").c_str());
      glUniform1i(tex, i);
      
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, light->depthMap_);

      ++i;

      glErrorCheck();

      if (i >= MAX_DIRECTIONAL_LIGHTS) return;
    }
  }

  void DirectionalLight::PrepareLightDepthBuffers(vec3 viewer)
  {

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    shadowMapper_->Use();

    for (auto light : GetList())
    {
      glBindFramebuffer(GL_FRAMEBUFFER, light->depthMapFBO_);
      glClear(GL_DEPTH_BUFFER_BIT);

      GLfloat near_plane = 1.0f;
      GLfloat far_plane = light->shadow_distance;

      Transform* transform = light->Owner->GetComponent<Transform>();
      vec3 lightPos = transform->position;
      vec3 direction = transform->GetForwardDirection();

      mat4 lightProj = ortho(-light->shadow_distance, 
                              light->shadow_distance, 
                             -light->shadow_distance,
                              light->shadow_distance,
                              near_plane, far_plane);
      mat4 lightView = lookAt(
        viewer - (direction * light->shadow_distance * 0.5f),
        viewer + (direction * light->shadow_distance),
        vec3(0, 0, 1));

      light->lightSpaceTransform_ = lightProj * lightView;
      
      shadowMapper_->Use();
      shadowMapper_->SetUniformMat4f("lightSpaceTransform", light->lightSpaceTransform_);

      glErrorCheck();

      glCullFace(GL_FRONT);
      CubeMesh::DrawShapeOnly(shadowMapper_);

      glCullFace(GL_BACK);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    glViewport(0, 0, engine_->GetGameWindow().Width(), engine_->GetGameWindow().Height());
  }

  void DirectionalLight::InitRendering(MagicBeansEngine * engine)
  {
    engine_ = engine;

    shadowMapper_ = new Shader("Resources/Shaders/directionallight_shadow.frag", "Resources/Shaders/directionallight_shadow.vert");
  }

}
