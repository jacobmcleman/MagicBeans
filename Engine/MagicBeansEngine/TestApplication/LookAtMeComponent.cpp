#include "stdafx.h"
#include "LookAtMeComponent.h"
#include "GameObject.h"

INIT_REFLECTION(LookAtMe);

LookAtMe::LookAtMe(GameObject* owner) : Component(owner)
{
}

void LookAtMe::SetCamera(Camera * cam)
{
  cam_ = cam;
}

void LookAtMe::Update(double dt)
{
  cam_->LookAt(Owner->GetComponent<Transform>()->position);

  (void)dt;
}

void LookAtMe::UpdateLookAtMes(double dt)
{
  for (auto& lam : GetList())
  {
    lam->Update(dt);
  }
}
