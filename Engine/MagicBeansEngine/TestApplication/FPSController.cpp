#include "stdafx.h"
#include "FPSController.h"
#include "GameObject.h"
#include "InputHandler.h"
#include "MathFunctions.h"

#include <../SDL2/include/SDL.h>

INIT_REFLECTION(FPSController);

FPSController::FPSController(GameObject * owner) : Component(owner), speed(30)
{
  transform_ = Owner->AddComponent<Transform>();
  camera_ = Owner->AddComponent<Camera>();

  yaw_ = 0.0f;
  pitch_ = 0.0f;
}

void FPSController::UpdateFPSControllers(double dt)
{
  for (auto& toUpdate : GetList())
  {
    toUpdate->Update(dt);
  }
}

void FPSController::Update(double dt)
{
  using namespace glm;

  ////////////////////////////////////////
  //    MOVEMENT
  ////////////////////////////////////////
  vec3 vel;

  if (InputHandler::IsKeyDown(SDL_SCANCODE_UP) || InputHandler::IsKeyDown(SDL_SCANCODE_W))
  {
    vel = vel + camera_->LookDirection.Get();
    vel.z = 0;
  }
  if (InputHandler::IsKeyDown(SDL_SCANCODE_DOWN) || InputHandler::IsKeyDown(SDL_SCANCODE_S))
  {
    vel = vel - camera_->LookDirection.Get();
    vel.z = 0;
  }
  if (InputHandler::IsKeyDown(SDL_SCANCODE_LEFT) || InputHandler::IsKeyDown(SDL_SCANCODE_A))
  {
    vel = vel - cross(camera_->LookDirection.Get(), camera_->GlobalUp.Get());
    vel.z = 0;
  }
  if (InputHandler::IsKeyDown(SDL_SCANCODE_RIGHT) || InputHandler::IsKeyDown(SDL_SCANCODE_D))
  {
    vel = vel + cross(camera_->LookDirection.Get(), camera_->GlobalUp.Get());
    vel.z = 0;
  }

  if (MagnitudeSquared(vel) > 0)
  {
    vel = normalize(vel);
    vel *= dt * speed;

    transform_->position += vel;
  }

  ////////////////////////////////////////
  //    ROTATION
  ////////////////////////////////////////
  vec2 mouseMovement = InputHandler::GetMouseMotion();
 
    if(abs(mouseMovement.x) > 0.05f) yaw_ += mouseMovement.x;

   if (abs(mouseMovement.y) > 0.05f)
   {
       if (InvertY) pitch_ += mouseMovement.y;
       else pitch_ -= mouseMovement.y;
   }

  if (pitch_ > c_max_pitch_)
  {
    pitch_ = c_max_pitch_;
  }
  else if (pitch_ < -c_max_pitch_)
  {
    pitch_ = -c_max_pitch_;
  }

  camera_->Owner->GetComponent<Transform>()->rotation = vec3(-radians(pitch_), 0, -radians(yaw_));
}
