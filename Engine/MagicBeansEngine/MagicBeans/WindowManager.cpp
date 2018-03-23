#include "WindowManager.h"
#include "Logger.h"

#include <../glad/include/glad/glad.h>
#include <../SDL2/include/SDL.h>

#include <assert.h>
#include <iostream>
#include <string>
#include <functional>

#include "InputHandler.h"
#include "Helpers.h"

namespace Beans
{
  WindowManager::WindowManager(const std::string& title, CursorMode cursor, int width, int height) :
    window_(nullptr),
    width_(width), height_(height), lastFrameTime_(std::chrono::system_clock::now())
  {
    LOG("Creating window manager");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        LOG("Failed to initialize SDL");
        throw std::runtime_error("SDL Initialization Failed!");
    }

    SDL_GL_LoadLibrary(nullptr); //Load default openGL library

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

    switch (cursor)
    {
    case CursorMode::Hidden:
        SDL_SetRelativeMouseMode(SDL_bool::SDL_TRUE);
        SDL_CaptureMouse(SDL_bool::SDL_FALSE);
        break;
    case CursorMode::Disabled:
        SDL_SetRelativeMouseMode(SDL_bool::SDL_TRUE);
        SDL_CaptureMouse(SDL_bool::SDL_TRUE);
        break;
    default:
        SDL_SetRelativeMouseMode(SDL_bool::SDL_FALSE);
        SDL_CaptureMouse(SDL_bool::SDL_FALSE);
        break;
    }

    if (window_ == nullptr)
    {
      LOG("Window creation failed");
      throw std::runtime_error("Window Creation Failed!");
    }

    LOG("OpenGL loaded\n");
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    
    SDL_GLContext glContext = SDL_GL_CreateContext(window_);
    
    if (glContext == nullptr)
    {
        LOG("GL Context creation failed");
        throw std::runtime_error("GL Context Failed!");
    }

    LOG(std::string("Vendor:   ") + reinterpret_cast<const char*>(glGetString(GL_VENDOR)))   ;
    LOG(std::string("Renderer: ") + reinterpret_cast<const char*>(glGetString(GL_RENDERER))) ;
    LOG(std::string("Version:  ") + reinterpret_cast<const char*>(glGetString(GL_VERSION)))  ;

    //Vysnc on
    SDL_GL_SetSwapInterval(1);

   
    SDL_GetWindowSize(window_, &width_, &height_);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        LOG("Failed to initialize GLAD");
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, width_, height_);
    glClearColor(0.0f, 0.5f, 1.0f, 0.0f);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);

    LOG("Window created");
  }

  WindowManager::~WindowManager()
  {
      SDL_DestroyWindow(window_);
      SDL_Quit();
  }

  bool WindowManager::UpdateWindow()
  {
      bool exit = false;
      SDL_Event e;

      while (SDL_PollEvent(&e) != 0)
      {
          switch(e.type)
          {
          case SDL_QUIT:
              exit = true;
              break;
          case SDL_KEYDOWN:
              //TODO add modifier key handling
              InputHandler::KeyCallback(e.key.keysym.sym, e.key.keysym.scancode, KeyAction::Pressed, 0);
              break;
          case SDL_KEYUP:
              //TODO add modifier key handling
              InputHandler::KeyCallback(e.key.keysym.sym, e.key.keysym.scancode, KeyAction::Released, 0);
              break;
          case SDL_MOUSEMOTION:
              InputHandler::MouseCallback(e.motion.x, e.motion.y);
              break;
          }
      }

    return !exit;
  }

  void WindowManager::SwapBuffers()
  {
    SDL_GL_SwapWindow(window_);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    //glClear(GL_COLOR_BUFFER_BIT);
  }
  double WindowManager::GetDeltaTime()
  {
    time_point currentFrame = std::chrono::system_clock::now();
    double deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentFrame - lastFrameTime_).count() * 0.000001;
    lastFrameTime_ = currentFrame;
    return deltaTime;
  }
}