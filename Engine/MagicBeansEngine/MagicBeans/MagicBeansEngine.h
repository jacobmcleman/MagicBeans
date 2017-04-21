#pragma once

#include <string>
#include "WindowManager.h"

#include "GameObject.h"
#include <functional>

#define GLM_FORCE_CXX11
#include "GLM\glm.hpp"

namespace Beans
{
  // This is an example of a class exported from the MagicBeansEngine.lib
  class MagicBeansEngine
  {
  public:
    typedef std::function<void(glm::mat4)> DrawFunction;
    typedef std::function<void(double)> UpdateFunction;

    MagicBeansEngine(const std::string& gamename);

    void RunGameLoop();

    void RegisterUpdateFunction(UpdateFunction function);
    void RegisterDrawFunction(DrawFunction function);

    GameObject* CreateObject(const std::string& name);

    GameObject* GetCamera();

  private:
    void UpdateStep();
    void DrawStep();
    void DeleteStep();

    double timeElapsed_;
    WindowManager gameWindow_;
    std::string gameName_;

    GameObject* cameraObject_;

    std::vector<GameObject> objects_;
    std::vector<DrawFunction> drawFunctions_;
    std::vector<UpdateFunction> updateFunctions_;
  };

}