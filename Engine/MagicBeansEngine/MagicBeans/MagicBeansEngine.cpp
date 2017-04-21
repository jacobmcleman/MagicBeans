// MagicBeansEngine.cpp : Defines the exported functions for the static library.
//

#include "MagicBeansEngine.h"
#include "Logger.h"

#include "Sprite.h"
#include "Camera.h"

namespace Beans
{
  // This is the constructor of a class that has been exported.
  // see MagicBeansEngine.h for the class definition
  MagicBeansEngine::MagicBeansEngine(const std::string& gamename) :
    gameWindow_(gamename)
  {
    

  }

  void MagicBeansEngine::RunGameLoop()
  {
    while (gameWindow_.UpdateWindow())
    {
      UpdateStep();

      DrawStep();

      gameWindow_.SwapBuffers();
    }
  }

  void MagicBeansEngine::UpdateStep()
  {
    double dt = gameWindow_.GetDeltaTime();
    (void)dt;
    //Do nothing (for now)
  }

  void MagicBeansEngine::DrawStep()
  {
    if (cameraObject_ != nullptr)
    {
      Camera* cam = cameraObject_->GetComponent<Camera>();
      if (cam != nullptr)
      {
        cam->UpdateMatrix();
        Sprite::DrawSprites(cam->GetMatrix());
      }
    }
    
  }

}