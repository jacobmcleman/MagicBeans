#pragma once
#include "Component.h"
#include "Transform.h"

using namespace Beans;

class IdleMovement : public Component, protected Utilities::AutoLister<IdleMovement>
{
public:
  IdleMovement(GameObject* owner);

  REFLECT_CLASS;

  static void UpdateIdleMovers(double dt);
  void Update(double dt);

  vec3 moveMagnitude;
  vec3 moveFrequency;
  vec3 basePosition;
private:
  Transform* transform_;
  float timer_;
};