#include "Transform.h"
#include <GLM\gtc\matrix_transform.hpp>

namespace Beans
{
  INIT_REFLECTION(Transform);

  Transform::Transform(GameObject * owner) :
    Component(owner),
    position(vec3(0, 0, 0), 
      [&](const vec3& input)-> vec3 
      { 
        needsUpdate_ = true; 
        return input; 
      }),
    rotation(vec3(0, 0, 0),
      [&](const vec3& input)->vec3 
      { 
        needsUpdate_ = true; 
        rotation_ = quat(input);
        return input;
      }),
    scale(vec3(10, 10, 10), 
      [&](const vec3& input)->vec3 
      { 
        needsUpdate_ = true; 
        return input; 
      })
  {
  }

  const mat4& Transform::GetMatrix()
  {
    if (needsUpdate_)
    {
      matrix_ = mat4();
      matrix_ = glm::translate(matrix_, static_cast<vec3>(position));
      matrix_ *= rotation_.operator glm::tmat4x4<float, glm::highp>();
      matrix_ = glm::scale(matrix_, scale.Get());
    }

    return matrix_;
  }
  void Transform::RotateBy(vec3 eulerAngles)
  {
    rotation += eulerAngles;
  }
  vec3 Transform::GetForwardDirection()
  {
    return (rotation_.operator glm::tmat3x3<float, glm::highp>()) * vec3(0, 1, 0);
  }
  quat Transform::GetRotationQuaternion() const
  {
    return rotation_;
  }
}