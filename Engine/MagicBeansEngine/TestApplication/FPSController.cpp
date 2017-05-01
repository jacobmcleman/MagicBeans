#include "stdafx.h"
#include "FPSController.h"
#include "GameObject.h"
#include "InputHandler.h"
#include "GLFW\glfw3.h"

INIT_REFLECTION(FPSController);

FPSController::FPSController(GameObject * owner) : Component(owner), speed(10)
{
  transform_ = Owner->AddComponent<Transform>();
  camera_ = Owner->AddComponent<Camera>();
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

  vec3 vel;

  if (InputHandler::IsKeyDown(GLFW_KEY_UP) || InputHandler::IsKeyDown(GLFW_KEY_W))
  {
    vel = vel + camera_->LookDirection.Data();
    vel.z = 0;
  }
  if (InputHandler::IsKeyDown(GLFW_KEY_DOWN) || InputHandler::IsKeyDown(GLFW_KEY_S))
  {
    vel = vel - camera_->LookDirection.Data();
    vel.z = 0;
  }
  if (InputHandler::IsKeyDown(GLFW_KEY_LEFT) || InputHandler::IsKeyDown(GLFW_KEY_A))
  {
    vel = vel - cross(camera_->LookDirection.Data(), camera_->GlobalUp.Data());
    vel.z = 0;
  }
  if (InputHandler::IsKeyDown(GLFW_KEY_RIGHT) || InputHandler::IsKeyDown(GLFW_KEY_D))
  {
    vel = vel + cross(camera_->LookDirection.Data(), camera_->GlobalUp.Data());
    vel.z = 0;
  }

  vel *= dt * speed;

  transform_->position += vel;
}
