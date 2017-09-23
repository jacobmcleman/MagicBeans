#include "VoxelObject.h"

namespace Beans
{
  VoxelObject::VoxelObject(GameObject * owner) : Component(owner), blocks(nullptr), blockSize(1.0f)
  {
  }

  VoxelObject::~VoxelObject()
  {
    if (blocks != nullptr)
    {
      delete blocks;
    }
  }

  void VoxelObject::SetupObject(unsigned short x, unsigned short y, unsigned short z)
  {
    if (blocks != nullptr)
    {
      delete blocks;
    }

    xSize = x;
    ySize = y;
    zSize = z;

    blocks = new Volume<unsigned short>(x, y, z);
  }

  void VoxelObject::DrawObject()
  {
    //Add all blocks to the draw queue
    for (int x = 0; x < xSize; ++x)
    {
      for (int y = 0; y < ySize; ++y)
      {
        for (int z = 0; z < zSize; ++z)
        {
          //TODO add to this condition later to cull blocks that are not visible
          if (blocks->At(x, y, z) != 0)
          {
            positions.push_back(vec3(x, y, z));
            materials.push_back(blocks->At(x, y, z));
          }
        }
      }
    }

    //Instanced draw the cube mesh
    
  }
}