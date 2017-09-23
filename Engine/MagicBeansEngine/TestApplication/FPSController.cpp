#include "stdafx.h"
#include "FPSController.h"
#include "GameObject.h"
#include "InputHandler.h"
#include "MathFunctions.h"

#include "GLFW\glfw3.h"

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

  if (InputHandler::IsKeyDown(GLFW_KEY_UP) || InputHandler::IsKeyDown(GLFW_KEY_W))
  {
    vel = vel + camera_->LookDirection.Get();
    vel.z = 0;
  }
  if (InputHandler::IsKeyDown(GLFW_KEY_DOWN) || InputHandler::IsKeyDown(GLFW_KEY_S))
  {
    vel = vel - camera_->LookDirection.Get();
    vel.z = 0;
  }
  if (InputHandler::IsKeyDown(GLFW_KEY_LEFT) || InputHandler::IsKeyDown(GLFW_KEY_A))
  {
    vel = vel - cross(camera_->LookDirection.Get(), camera_->GlobalUp.Get());
    vel.z = 0;
  }
  if (InputHandler::IsKeyDown(GLFW_KEY_RIGHT) || InputHandler::IsKeyDown(GLFW_KEY_D))
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

	if (MagnitudeSquared(mouseMovement) > 0.005f)
	{
		yaw_ += mouseMovement.x;


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
