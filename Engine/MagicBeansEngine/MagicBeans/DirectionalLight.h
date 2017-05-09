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
  class MagicBeansEngine;

  class DirectionalLight : public Component, protected Utilities::AutoLister<DirectionalLight>
  {
  public:
    REFLECT_CLASS;

    DirectionalLight(GameObject* owner);
    virtual ~DirectionalLight();

    vec3 color;
    float shadow_distance;

    static void SendLightsToShader(Shader* shader);
    static void PrepareLightDepthBuffers(vec3 viewer);

    static void InitRendering(MagicBeansEngine* engine);

  private:
    Transform* transform_;

    GLuint depthMapFBO_;
    GLuint depthMap_;
    mat4 lightSpaceTransform_;

    static MagicBeansEngine* engine_;
    static Shader* shadowMapper_;

    const static unsigned int SHADOW_WIDTH = 4096;
    const static unsigned int SHADOW_HEIGHT = 4096;
  };
}