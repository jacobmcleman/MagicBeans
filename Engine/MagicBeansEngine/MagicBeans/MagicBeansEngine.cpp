// MagicBeansEngine.cpp : Defines the exported functions for the static library.
//

#include "MagicBeansEngine.h"
#include "Logger.h"
#include "InputHandler.h"
#include "Sprite.h"
#include "CubeMesh.h"
#include "Camera.h"
#include "JobData.h"
#include "PointLight.h"
#include "DirectionalLight.h"

namespace
{
  //Struct for the engine to use to send the data to the non-member job function
  struct EngineRunUpdateJobData
  {
    std::vector<JobFunction>* jobs;
    float dt;
  };
}

namespace Beans
{
  // This is the constructor of a class that has been exported.
  // see MagicBeansEngine.h for the class definition
  MagicBeansEngine::MagicBeansEngine(const std::string& gamename, CursorMode cursorMode) :
    gameWindow_(gamename, cursorMode, 1920, 1080), cameraObject_(nullptr), timeElapsed_(0)
  {
    /*
      Use the default constructor for workers which will automatically
      use all of the threads in the machine
    */

    //Automatically register sprite's draw functions
    RegisterDrawFunction(Sprite::DrawSprites);
    RegisterDrawFunction(CubeMesh::DrawSprites);

    //RegisterPreDrawFunction(PointLight::ComputeDepthMapsForShadows);
    RegisterPreDrawFunction(DirectionalLight::PrepareLightDepthBuffers);

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

  void MagicBeansEngine::RegisterPreDrawFunction(PreDrawFunction function)
  {
    earlyDrawFunctions_.push_back(function);
  }

  void MagicBeansEngine::RegisterUpdateJob(JobFunction job)
  {
    updateJobs_.push_back(job);
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

  const WindowManager& MagicBeansEngine::GetGameWindow() const
  {
    return gameWindow_;
  }

  void MagicBeansEngine::UpdateStep()
  {
    double dt = gameWindow_.GetDeltaTime();
    timeElapsed_ += dt;

    ////Start all currently registered update jobs
    //EngineRunUpdateJobData updateJobData;
    //updateJobData.dt = static_cast<float>(dt);
    //updateJobData.jobs = &updateJobs_;
    //
    ////Start the execution of the update jobs
    //Job* updateJob = CreateJob(RunUpdateJobs, updateJobData);
    //workers_.Run(updateJob);

    //Call all currently registered synchronus update functions
    for (MagicBeansEngine::UpdateFunction func : updateFunctions_)
    {
      func(dt);
    }

    //Wait for the update jobs to be finished
    //workers_.Wait(updateJob);
  }

  void MagicBeansEngine::DrawStep()
  {
    if (cameraObject_ != nullptr)
    {
      Camera* cam = cameraObject_->GetComponent<Camera>();
      if (cam != nullptr)
      {
        vec3 camPos = cameraObject_->GetComponent<Transform>()->position;
        cam->UpdateMatrix();

        //Call early draw functions for things such as shadow computation
        for (MagicBeansEngine::PreDrawFunction func : earlyDrawFunctions_)
        {
          func(camPos);
        }

        //Finally, draw the scene from the camera's perspective
        DrawScene(cam->GetMatrix());
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
    PointLight::InitRendering(this);
    DirectionalLight::InitRendering(this);
  }

  void MagicBeansEngine::DrawScene(mat4 projection)
  {
    //Call all currently registered draw functions
    for (MagicBeansEngine::DrawFunction func : drawFunctions_)
    {
      func(projection);
    }
  }

  void MagicBeansEngine::RunUpdateJobs(Job * job, void * uncast_data)
  {
    EngineRunUpdateJobData* data = reinterpret_cast<EngineRunUpdateJobData*>(uncast_data);

    UpdateJobData updateData;
    updateData.deltaTime = data->dt;

    for (auto jobFunc : *(data->jobs))
    {
      job->RunChild(CreateJob(jobFunc, updateData, nullptr));
    }
  }

  

}