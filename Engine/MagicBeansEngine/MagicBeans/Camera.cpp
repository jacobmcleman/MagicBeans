#include "Camera.h"

#include "GameObject.h"
#include "MathFunctions.h"

#define PERSPECTIVE

namespace Beans
{
  INIT_REFLECTION(Camera);

  Camera::Camera(GameObject * owner) : Component(owner), size(30, 30),
    GlobalUp(vec3(0, 0, 1), 
      //Setter (normalize before setting)
      [&](const vec3& input)->vec3
      { 
        return normalize(input); 
      }),
    LookDirection(vec3(0, 1, 0),
      //Setter (does nothing, just pass thru. Should actually turn cam object)
      [&](const vec3& input)->vec3 
      { 
        return input;
      },
      //Getter (calculate what the value is expected to be and return)
      [&](const vec3& curr)->vec3 
      {
        (void)curr; //Don't use since is calculated;
        return Owner->GetComponent<Transform>()->GetRotationQuaternion() * vec3(0, 1, 0);  
      })
  {
  }

  void Camera::LookAt(const vec3 & position)
  {
    (void)position;
    //TODO actually implement this function
    //LookDirection = (position - Owner->GetComponent<Transform>()->position.Data());
  }

  void Camera::UpdateMatrix()
  {
#ifndef PERSPECTIVE
    float top, bottom, left, right;
    top = ((vec3)Owner->GetComponent<Transform>()->position).y + (size.y / 2);
    bottom = ((vec3)Owner->GetComponent<Transform>()->position).y - (size.y / 2);
    left = ((vec3)Owner->GetComponent<Transform>()->position).x - (size.x / 2);
    right = ((vec3)Owner->GetComponent<Transform>()->position).x + (size.x / 2);
    matrix_ = glm::ortho(left, right, bottom, top);
#else
    vec3 lookDir = Owner->GetComponent<Transform>()->GetRotationQuaternion() * vec3(0, 1, 0);
    mat4 view = 
      glm::lookAt(Owner->GetComponent<Transform>()->position.Get(), //Eye position
                  Owner->GetComponent<Transform>()->position + lookDir, //Camera center
                  GlobalUp.Get()); //Up vector

    mat4 projection = glm::perspective(45.0f, 1920.0f / 1080.0f, 0.1f, 250.0f);

    matrix_ = projection * view;
#endif
  }
  const mat4& Camera::GetMatrix()
  {
    return matrix_;
  }
}

