#include "PointLight.h"
#include "GameObject.h"
#include "Transform.h"
#include "Helpers.h"
#include "MagicBeansEngine.h"
#include <GLM\gtc\matrix_transform.hpp>
#include "CubeMesh.h"
#include <glm/gtc/type_ptr.hpp>

namespace Beans
{
  INIT_REFLECTION(PointLight);

  const float powerDepthMultiplier = 10.0f;

  Shader* PointLight::shadowMapper_ = nullptr;
  MagicBeansEngine* PointLight::engine_ = nullptr;

  PointLight::PointLight(GameObject * owner) : Component(owner), power(1), color(1, 1, 1)
  {
    transform_ = Owner->AddComponent<Transform>();

    //Create a cube map texture to store the surrounding depth values in
    //glGenTextures(1, &depthCubemap_);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap_);
    //
    ////Create a frame buffer object to use to draw depth values into the cube map
    //glGenFramebuffers(1, &depthMapFBO_);
    //
    ////Initialize 6 textures for each side of the cube map
    //for (GLuint i = 0; i < 6; ++i)
    //{
    //  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    //}
    //
    ////Set the cube map parameters
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    //
    ////Set up the frame buffer to render to the cubemap
    //glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_);
    //glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap_, 0);
    //glDrawBuffer(GL_NONE); //Only need to render depth values to this buffer
    //glReadBuffer(GL_NONE); //So disable draw and read
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

      shader->SetUniformVec3f((uniformName + "position").c_str(), light->transform_->position);
      shader->SetUniformVec3f((uniformName + "color").c_str(), light->color * light->power * 10.0f);
      //shader->SetUniformFloat((uniformName + "max_distance").c_str(), powerDepthMultiplier * light->power);
     
      glErrorCheck();

      

      //Textures 0-MAX_POINT_LIGHTS will be used for depth cubemaps
      //GLint tex = shader->getUniformLoc((uniformName + "depthMap").c_str());
      //glUniform1i(tex, i);
      //
      //glActiveTexture(GL_TEXTURE0 + i);
      //glBindTexture(GL_TEXTURE_CUBE_MAP, light->depthCubemap_);

      ++i;

      if (i >= MAX_POINT_LIGHTS) return;
    }
  }

  void PointLight::ComputeDepthMapsForShadows(mat4 camMatrix)
  {
    (void)camMatrix;

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
    

    for (PointLight* light : GetList())
    {
      GLfloat far = powerDepthMultiplier * light->power;
      GLfloat near = 0.1f;

      mat4 shadowProj = perspective(radians(90.0f), aspect, near, far);

      glBindFramebuffer(GL_FRAMEBUFFER, light->depthMapFBO_);
      glErrorCheck();
      glClear(GL_DEPTH_BUFFER_BIT);
      glErrorCheck();
      
      glErrorCheck();

      vec3 lightPos = light->Owner->GetComponent<Transform>()->position;

      mat4 shadowTransforms[6];
      shadowTransforms[0] = shadowProj *
        glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
      shadowTransforms[1] = shadowProj *
        glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
      shadowTransforms[2] = shadowProj *
        glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
      shadowTransforms[3] = shadowProj *
        glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
      shadowTransforms[4] = shadowProj *
        glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
      shadowTransforms[5] = shadowProj *
        glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));
      
      GLfloat shadowTransformFloatData[6 * 16];
      for (int i = 0; i < 6; ++i)
      {
        memcpy(shadowTransformFloatData + (i * 16), value_ptr(shadowTransforms[i]), 16 * sizeof(GLfloat));
      }

      shadowMapper_->Use();

      //for (GLuint i = 0; i < 6; ++i)
      //{
      //  GLuint uniformLoc = shadowMapper_->getUniformLoc(("shadowMatrices[" + std::to_string(i) + "]").c_str()); glErrorCheck();
      //  glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(shadowTransforms[i])); glErrorCheck();
      //}
      glUniformMatrix4fv(shadowMapper_->getUniformLoc("shadowMatrices"), 6, GL_FALSE, shadowTransformFloatData); glErrorCheck();
      
      shadowMapper_->SetUniformFloat("far_plane", far); glErrorCheck();
      shadowMapper_->SetUniformVec3f("lightPos", lightPos); glErrorCheck();
      glErrorCheck();

      //Ask all cubes to be drawn as untextured depth-only objects
      CubeMesh::DrawShapeOnly(shadowMapper_);
      glErrorCheck();

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glViewport(0, 0, engine_->GetGameWindow().Width(), engine_->GetGameWindow().Height());
    }

    
  }

  void PointLight::InitRendering(MagicBeansEngine * engine)
  {
    engine_ = engine;

    shadowMapper_ = new Shader("Resources/Shaders/pointlight_shadow.frag", "Resources/Shaders/pointlight_shadow.vert", "Resources/Shaders/pointlight_shadow.geom");
    glErrorCheck();


  }
}