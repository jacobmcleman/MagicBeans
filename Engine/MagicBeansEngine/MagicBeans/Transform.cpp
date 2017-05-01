#include "Transform.h"
#include <GLM\gtc\matrix_transform.hpp>

namespace Beans
{
  INIT_REFLECTION(Transform);

  Transform::Transform(GameObject * owner) :
    Component(owner),
    position(vec3(0, 0, 0), [&](const vec3& input)-> vec3 { needsUpdate_ = true; return input; }),
    rotation(0, [&](const float& input)->float { needsUpdate_ = true; return input; }),
    scale(vec3(10, 10, 10), [&](const vec3& input)->vec3 { needsUpdate_ = true; return input; })
  {
  }

  const mat4& Transform::GetMatrix()
  {
    if (needsUpdate_)
    {
      matrix_ = mat4();
      matrix_ = glm::translate(matrix_, static_cast<vec3>(position));
      matrix_ = glm::rotate(matrix_, static_cast<float>(rotation), vec3(0, 0, 1));
      matrix_ = glm::scale(matrix_, scale.Data());
    }

    return matrix_;
  }
}