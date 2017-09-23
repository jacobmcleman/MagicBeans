#pragma once
#include "Component.h"
#include "Shader.h"
#include "AutoLister.h"
#include "MultiDimArray.h"


#include <vector>
#include "GLM\glm.hpp"

using namespace glm;
namespace Beans
{
  class MagicBeansEngine;

  class VoxelObject : public Component, protected Utilities::AutoLister<VoxelObject>
  {
  public:
    REFLECT_CLASS;

    VoxelObject(GameObject* owner);
    virtual ~VoxelObject();

    void SetupObject(unsigned short xSize, unsigned short ySize, unsigned short zSize);

    static void Draw();

    unsigned short xSize;
    unsigned short ySize;
    unsigned short zSize;

    const float blockSize;

    
  private:
    Volume<unsigned short>* blocks;

    std::vector<vec3> positions;
    std::vector<unsigned short> materials;

    static const float Vertices[];
    static Shader* shaderProgram;
    static unsigned int VBO;
    static unsigned int VAO;

    static MagicBeansEngine* engine_;

    void DrawObject();
  };
}