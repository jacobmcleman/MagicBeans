#pragma once
#include "Component.h"
#include "AutoLister.h"
#include "GLM\glm.hpp"

#include "Shader.h"

using namespace glm;

#define MAX_POINT_LIGHTS 16

namespace Beans
{
  class Transform;
  class MagicBeansEngine;

  class PointLight : public Component, protected Utilities::AutoLister<PointLight>
  {
  public:
    REFLECT_CLASS;

    PointLight(GameObject* owner);
    virtual ~PointLight();

    float power;
    vec3 color;

    static void SendLightsToShader(Shader* shader);

    static void ComputeDepthMapsForShadows(mat4 camMatrix);

    static void InitRendering(MagicBeansEngine* engine);

  private:
    Transform* transform_;

    GLuint depthCubemap_;
    GLuint depthMapFBO_;

    static MagicBeansEngine* engine_;

    const static GLuint SHADOW_WIDTH = 1024;
    const static GLuint SHADOW_HEIGHT = 1024;
    static Shader* shadowMapper_;
  };
}