// MagicBeansEngine.cpp : Defines the exported functions for the static library.
//

#include "MagicBeansEngine.h"
#include "Logger.h"
#include "InputHandler.h"
#include "Sprite.h"
#include "CubeMesh.h"
#include "Camera.h"

namespace Beans
{
  // This is the constructor of a class that has been exported.
  // see MagicBeansEngine.h for the class definition
  MagicBeansEngine::MagicBeansEngine(const std::string& gamename, CursorMode cursorMode) :
    gameWindow_(gamename, cursorMode, 1920, 1080), cameraObject_(nullptr), timeElapsed_(0)
  {
    //Automatically register sprite's draw functions
    RegisterDrawFunction(Sprite::DrawSprites);
    RegisterDrawFunction(CubeMesh::DrawSprites);

    SetupRendering();

    cameraObject_ = CreateObject("Main Camera");
    cameraObject_->AddComponent<Transform>();
    cameraObject_->AddComponent<Camera>();
  }

  void MagicBeansEngine::RunGameLoop()
  {
    while (gameWindow_.UpdateWindow())
    {
      InputHandler::UpdateInput();

      UpdateStep();

      DrawStep();

      DeleteStep();

      gameWindow_.SwapBuffers();
    }
  }

  void MagicBeansEngine::RegisterUpdateFunction(MagicBeansEngine::UpdateFunction function)
  {
    updateFunctions_.push_back(function);
  }

  void MagicBeansEngine::RegisterDrawFunction(MagicBeansEngine::DrawFunction function)
  {
    drawFunctions_.push_back(function);
  }

  GameObject * MagicBeansEngine::CreateObject(const std::string & name)
  {
    objects_.emplace_back(new GameObject(name));
    return objects_.back();
  }

  double MagicBeansEngine::GetTimeSinceStartup() const
  {
    return timeElapsed_;
  }

  GameObject * MagicBeansEngine::GetCamera()
  {
    return cameraObject_;
  }

  void MagicBeansEngine::UpdateStep()
  {
    double dt = gameWindow_.GetDeltaTime();
    timeElapsed_ += dt;
    (void)dt;
    

    //Call all currently registered update functions
    for (MagicBeansEngine::UpdateFunction func : updateFunctions_)
    {
      func(dt);
    }
  }

  void MagicBeansEngine::DrawStep()
  {
    if (cameraObject_ != nullptr)
    {
      Camera* cam = cameraObject_->GetComponent<Camera>();
      if (cam != nullptr)
      {
        cam->UpdateMatrix();

        //Call all currently registered update functions
        for (MagicBeansEngine::DrawFunction func : drawFunctions_)
        {
          func(cam->GetMatrix());
        }
      }
    }
    
  }

  void MagicBeansEngine::DeleteStep()
  {
    //Find all objects to delete
    for (int i = 0; i < objects_.size(); ++i)
    {
      if (objects_[i]->isMarkedForDelete())
      {
        std::swap(objects_[i], objects_.back());
        GameObject* deleted = objects_.back();
        objects_.pop_back();
        delete deleted;
      }
    }
  }

  void MagicBeansEngine::SetupRendering()
  {
    Sprite::InitRendering();
    CubeMesh::InitRendering(this);
  }

}