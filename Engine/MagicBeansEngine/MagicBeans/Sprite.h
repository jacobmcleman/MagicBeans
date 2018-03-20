#pragma once

#include "Component.h"
#include "AutoLister.h"

#include "..\Includes\GLM\glm.hpp"

#include "Shader.h"

using namespace glm;


namespace Beans
{
  class Sprite : public Component, protected Utilities::AutoLister<Sprite>
  {
  public:
    REFLECT_CLASS;

    Sprite(GameObject* owner);
    virtual ~Sprite();

    void Draw();

    static void InitRendering();
    static void DrawSprites(const mat4& camTransform);

    vec4 Color;
  
  private:
    static const float Vertices[];
    static const unsigned int Indices[];
    static Shader* shaderProgram;
    static unsigned int VBO;
    static unsigned int VAO;
    static unsigned int EBO;
  };
} 