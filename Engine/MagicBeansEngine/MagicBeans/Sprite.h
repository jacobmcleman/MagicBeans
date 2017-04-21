#pragma once

#include "Component.h"
#include "AutoLister.h"

#define GLM_FORCE_CXX11
#include "GLM\glm.hpp"

using namespace glm;


namespace Beans
{
  class Sprite : public Component, public Utilities::AutoLister<Sprite>
  {
  public:
    REFLECT_CLASS;

    Sprite(GameObject* owner);

    void Draw();

    static void InitRendering();
    static void DrawSprites(const mat4& camTransform);

    vec4 Color;
  
  private:
    static const float Vertices[];
    static unsigned int shaderProgram;
    static unsigned int VBO;
    static unsigned int VAO;
  };
} 