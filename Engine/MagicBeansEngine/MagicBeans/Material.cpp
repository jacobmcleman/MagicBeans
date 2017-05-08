#include "Material.h"

namespace Beans
{
  Material::Material(vec3 Albedo, float Metallic, float Roughness, float AO) :
    albedo(Albedo), metallic(Metallic), roughness(Roughness), ao(AO)
  {
  }

  void Material::Use(Shader * shader)
  {
    shader->SetUniformVec3f("material.albedo", albedo);
    shader->SetUniformFloat("material.metallic", metallic);
    shader->SetUniformFloat("material.roughness", roughness);
    shader->SetUniformFloat("material.ao", ao);
  }

  Material Material::Gems::Emerald	  (vec3(0.0215	, 0.1745	, 0.0215	), 0.6f, 0.1f, 0.0f);
  Material Material::Gems::Jade	      (vec3(0.135	  , 0.2225	, 0.1575	), 0.6f, 0.1f, 0.0f);
  Material Material::Gems::Obsidian	  (vec3(0.05375	, 0.05	  , 0.06625	), 0.6f, 0.1f, 0.0f);
  Material Material::Gems::Pearl	    (vec3(0.25	  , 0.20725	, 0.20725	), 0.6f, 0.1f, 0.0f);
  Material Material::Gems::Ruby	      (vec3(0.1745	, 0.01175	, 0.01175	), 0.6f, 0.1f, 0.0f);
  Material Material::Gems::Turquoise	(vec3(0.1	    , 0.18725	, 0.1745	), 0.6f, 0.1f, 0.0f);
  Material Material::Metals::Brass	  (vec3(0.329412, 0.223529, 0.027451), 0.99f, 0.3f, 0.0f);
  Material Material::Metals::Bronze	  (vec3(0.2125	, 0.1275	, 0.054),    0.99f, 0.3f, 0.0f);
  Material Material::Metals::Chrome	  (vec3(0.550	  , 0.556	  , 0.554),    0.99f, 0.3f, 0.0f);
  Material Material::Metals::Copper	  (vec3(0.955	, 0.637	, 0.538	),       0.99f, 0.3f, 0.0f);
  Material Material::Metals::Gold	    (vec3(1.0	, 0.766	, 0.336	),         0.99f, 0.3f, 0.0f);
  Material Material::Metals::Silver	  (vec3(0.972	, 0.960	, 0.915	),       0.99f, 0.3f, 0.0f);
  Material Material::Plastics::Black  (vec3(0.0,  0.0	, 0.0	    ),         0.1f, 0.8f,  0.0f);
  Material Material::Plastics::Cyan   (vec3(0.0,  0.8	, 0.8	  ),           0.1f, 0.8f,  0.0f);
  Material Material::Plastics::Green  (vec3(0.0,  0.95, 0.0),              0.1f, 0.8f,  0.0f);
  Material Material::Plastics::Red    (vec3(0.95, 0.0	, 0.0),              0.1f, 0.8f,  0.0f);
  Material Material::Plastics::White  (vec3(0.9,  0.9	, 0.9),              0.1f, 0.8f,  0.0f);
  Material Material::Plastics::Yellow	(vec3(0.8,  0.8, 0.0),               0.1f, 0.8f,  0.0f);
}