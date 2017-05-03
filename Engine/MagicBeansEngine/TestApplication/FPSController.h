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

  bool InvertY = false;

private:
  Transform* transform_;
  Camera* camera_;

  float yaw_;
  float pitch_;

  const float c_max_pitch_ = 89;
};