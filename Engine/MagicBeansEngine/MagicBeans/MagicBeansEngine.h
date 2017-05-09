#pragma once

#include <string>
#include "WindowManager.h"

#include "GameObject.h"
#include <functional>

#include "GLM\glm.hpp"

#include "JobForwardDeclarations.h"
#include "WorkStealingQueue.h"

namespace Beans
{
  // This is an example of a class exported from the MagicBeansEngine.lib
  class MagicBeansEngine
  {
  public:
    //Draw functions take the projection matrix that should be used
    typedef std::function<void(glm::mat4)> DrawFunction;
    //Pre-draw functions take the position of the viewer
    typedef std::function<void(glm::vec3)> PreDrawFunction;
    //Update functions take the time taken to present the previous frame
    typedef std::function<void(double)> UpdateFunction;

    MagicBeansEngine(const std::string& gamename, CursorMode cursorMode = CursorMode::Normal);

    void RunGameLoop();

    void RegisterUpdateFunction(UpdateFunction function);
    void RegisterDrawFunction(DrawFunction function);
    void RegisterPreDrawFunction(PreDrawFunction function);

    void RegisterUpdateJob(JobFunction job);

    GameObject* CreateObject(const std::string& name);

    double GetTimeSinceStartup() const;

    GameObject* GetCamera();

    const WindowManager& GetGameWindow() const;

    //External accessor function to allow other modules to draw the game
    //scene from different perspectives
    void DrawScene(glm::mat4 projection);

  private:
    void UpdateStep();
    void DrawStep();
    void DeleteStep();

    void SetupRendering();

    

    static void RunUpdateJobs(Job* job, void* data);

    double timeElapsed_;
    WindowManager gameWindow_;
    std::string gameName_;

    GameObject* cameraObject_;

    //WorkStealingQueue workers_;

    std::vector<GameObject*> objects_;

    std::vector<DrawFunction> drawFunctions_;
    std::vector<PreDrawFunction> earlyDrawFunctions_;

    std::vector<UpdateFunction> updateFunctions_;
    std::vector<JobFunction> updateJobs_;

    
  };

}