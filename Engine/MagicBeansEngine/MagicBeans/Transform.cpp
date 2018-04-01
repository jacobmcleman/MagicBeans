#include "Transform.h"

#include <..\Includes\GLM\gtc\matrix_transform.hpp>

namespace Beans
{
  INIT_REFLECTION(Transform);


  Transform::Transform(GameObject * owner) :
      Component(owner),
      Position(this, "Position", &Transform::Position, &Transform::GetPosition, &Transform::SetPosition, vec3(0)),
    Rotation(this, "Rotation", &Transform::Rotation, &Transform::GetRotation, &Transform::SetRotation, vec3(0)),
    Scale(this, "Scale", &Transform::Scale, &Transform::GetScale, &Transform::SetScale, vec3(1))
  {
  }

  const mat4& Transform::GetMatrix() const
  {

    if (needsUpdate_)
    {
      matrix_ = mat4();
      matrix_ = glm::translate(matrix_, static_cast<vec3>(Position));
      matrix_ *= glm::mat4_cast(rotationQuat_);
      matrix_ = glm::scale(matrix_, Scale.Get());
    }

    return matrix_;
  }
  void Transform::RotateBy(vec3 eulerAngles)
  {
    Rotation += eulerAngles;
  }
  vec3 Transform::GetForwardDirection()
  {
    return (rotationQuat_.operator glm::tmat3x3<float, glm::highp>()) * vec3(0, 1, 0);
  }
  quat Transform::GetRotationQuaternion() const
  {
    return rotationQuat_;
  }
  vec3 Transform::GetPosition() const
  {
      return position_;
  }
  const vec3& Transform::SetPosition(const vec3 & aPosition)
  {
      needsUpdate_ = true;
      position_ = aPosition;
      return position_;
  }
  vec3 Transform::GetRotation() const
  {
      return rotationEuler_;
  }
  const vec3& Transform::SetRotation(const vec3 & aRotation)
  {
      needsUpdate_ = true;
      rotationEuler_ = aRotation;
      rotationQuat_ = quat(aRotation);
      return rotationEuler_;
  }
  vec3 Transform::GetScale() const
  {
      return scale_;
  }
  const vec3& Transform::SetScale(const vec3 & aScale)
  {
      needsUpdate_ = true;
      scale_ = aScale;
      return scale_;
  }
}