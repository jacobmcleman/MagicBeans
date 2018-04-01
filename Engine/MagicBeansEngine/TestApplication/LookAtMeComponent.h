#pragma once
#include "Component.h"
#include "Camera.h"

using namespace Beans;

class LookAtMe : public Component, protected Utilities::AutoLister<LookAtMe>
{
public:
  REFLECT_CLASS(LookAtMe);

  LookAtMe(GameObject* owner);

  void SetCamera(Camera* cam);
  void Update(double dt);

  static void UpdateLookAtMes(double dt);

private:
  Camera* cam_;
};