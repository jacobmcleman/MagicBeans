#pragma once
#include <GLM\glm.hpp>
using namespace glm;

#include "Shader.h"

namespace Beans
{
  class Material
  {
  public:
    //Constructor is not suggested for use except for maybe serialization and default material setup
    Material(vec3 ambient, vec3 diffuse, vec3 specular, float shininess);

    struct Gems
    {
      static Material Emerald;
      static Material Jade;
      static Material Obsidian;
      static Material Pearl;
      static Material Ruby;
      static Material Turquoise;
    };
    struct Metals
    {
      static Material Brass;
      static Material Bronze;
      static Material Chrome;
      static Material Copper;
      static Material Gold;
      static Material Silver;
    };
    struct Plastics
    {
      static Material Black;
      static Material Cyan;
      static Material Green;
      static Material Red;
      static Material White;
      static Material Yellow;
    };

    void Use(Shader* shader);

  private:
    vec3 ambient_;
    vec3 diffuse_;
    vec3 specular_;
    float shininess_;
  };
}