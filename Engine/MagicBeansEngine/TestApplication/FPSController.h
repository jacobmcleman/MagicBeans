#pragma once
#include "Component.h"
#include "Transform.h"
#include "Camera.h"

using namespace Beans;

class FPSController : public Component, protected Utilities::AutoLister<FPSController>
{
public:
  FPSController(GameObject* owner);

  REFLECT_CLASS;

  static void UpdateFPSControllers(double dt);
  void Update(double dt);

  double speed;

private:
  Transform* transform_;
  Camera* camera_;
};