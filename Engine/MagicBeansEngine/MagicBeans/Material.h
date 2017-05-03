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
    Material(vec3 ambient, vec3 diffuse, vec3 specular, float shininess, vec3 emission);

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
    struct SelfIlluminated
    {
      static Material WhiteLight;
      static Material RedLight;
      static Material GreenLight;
      static Material BlueLight;
    };

    void Use(Shader* shader);

  private:
    vec3 ambient_;
    vec3 diffuse_;
    vec3 specular_;
    float shininess_;
    vec3 emission_;
  };
}