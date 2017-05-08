#pragma once
#include "Component.h"
#include "AutoLister.h"
#include "Material.h"

#include "GLM\glm.hpp"

#include "Shader.h"


using namespace glm;

namespace Beans
{
  class MagicBeansEngine;

  class CubeMesh : public Component, protected Utilities::AutoLister<CubeMesh>
  {
  public:
    REFLECT_CLASS;

    CubeMesh(GameObject* owner);
    virtual ~CubeMesh();

    void Draw(Shader* shader);

    static void InitRendering(MagicBeansEngine* engine);
    static void DrawSprites(const mat4& camTransform);
    static void DrawShapeOnly(Shader* shader);

    Material material;

  private:
    static const float Vertices[];
    static Shader* shaderProgram;
    static unsigned int VBO;
    static unsigned int VAO;

    static MagicBeansEngine* engine_;
  };
}