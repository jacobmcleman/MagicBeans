#include "stdafx.h"
#include "IdleMovement.h"
#include "GameObject.h"

INIT_REFLECTION(IdleMovement);

IdleMovement::IdleMovement(GameObject * owner) : Component(owner), timer_(0)
{
  transform_ = Owner->AddComponent<Transform>();
}

void IdleMovement::UpdateIdleMovers(double dt)
{
  for (auto mover : GetList())
  {
    mover->Update(dt);
  }
}

void IdleMovement::Update(double dt)
{
  timer_ += (float)dt;

  vec3 offset;
  offset.x = moveMagnitude.x * sinf(moveFrequency.x * 2 * pi<float>() * timer_);
  offset.y = moveMagnitude.y * sinf(moveFrequency.y * 2 * pi<float>() * timer_);
  offset.z = moveMagnitude.z * sinf(moveFrequency.z * 2 * pi<float>() * timer_);

  transform_->position = basePosition + offset;
}
