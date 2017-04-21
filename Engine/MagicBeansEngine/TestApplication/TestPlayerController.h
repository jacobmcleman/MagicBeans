#pragma once
#include "Component.h"
#include "Transform.h"
#include "Sprite.h"

using namespace Beans;

class PlayerController : public Component, public Utilities::AutoLister<PlayerController>
{
public:
  PlayerController(GameObject* owner);

  REFLECT_CLASS;

  static void UpdatePlayerControllers(double dt);
  void Update(double dt);

  double speed;
private:
  Transform* transform_;
  Sprite* sprite_;
};