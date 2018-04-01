#include "stdafx.h"
#include "TestPlayerController.h"

#include "Component.h"
#include "GameObject.h"

#include "CubeMesh.h"
#include "Transform.h"

#include "InputHandler.h"

#include <../SDL2/include/SDL.h>

INIT_REFLECTION(PlayerController)

PlayerController::PlayerController(GameObject * owner) : Component(owner)
{
  speed = 10;
  sprite_ = Owner->AddComponent<CubeMesh>();
  transform_ = Owner->AddComponent<Transform>();

  sprite_->material = Material::Plastics::Red;
}

void PlayerController::Update(double dt)
{
 // sprite_ = Owner->GetComponent<CubeMesh>();
 // transform_ = Owner->GetComponent<Transform>();

  //Spin?
  //transform_->rotation += static_cast<float>(dt * 0.5);

  if (InputHandler::IsKeyDown(SDL_SCANCODE_LEFT))
  {
    transform_->Position += vec3(-dt * speed , 0, 0);
  }
  if (InputHandler::IsKeyDown(SDL_SCANCODE_RIGHT))
  {
    transform_->Position += vec3(dt * speed, 0, 0);
  }
  if (InputHandler::IsKeyDown(SDL_SCANCODE_UP))
  {
    transform_->Position += vec3(0, dt * speed, 0);
  }
  if (InputHandler::IsKeyDown(SDL_SCANCODE_DOWN))
  {
    transform_->Position += vec3(0, -dt * speed, 0);
  }
}

void PlayerController::UpdatePlayerControllers(double dt)
{
  for (auto& pc : GetList())
  {
    pc->Update(dt);
  }
}