#include "WindowManager.h"
#include "Logger.h"

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW\glfw3.h>

#include <assert.h>
#include <iostream>
#include <string>
#include <functional>

#include "InputHandler.h"
#include "Helpers.h"

namespace Beans
{
  WindowManager::WindowManager(const std::string& title, CursorMode cursor, int width, int height) :
    width_(width), height_(height), lastFrameTime_(0)
  {
    LOG("Creating window manager");
    glfwInit(); //glErrorCheck();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //glErrorCheck();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //glErrorCheck();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //glErrorCheck();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);//glErrorCheck();

    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (window_ == nullptr)
    {
      LOG("Window creation failed");
    }

    glfwMakeContextCurrent(window_);

    glewExperimental = GL_TRUE;

    glEnable(GL_DEPTH_TEST); glErrorCheck();

    GLenum initResult = glewInit();
    if (initResult != GLEW_OK)
    {
      LOG(((const char*)glewGetErrorString(initResult)));
    }

    glfwGetFramebufferSize(window_, &width_, &height_);

    glViewport(0, 0, width_, height_);

    switch (cursor)
    {
    case CursorMode::Disabled:
      glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      break;
    case CursorMode::Hidden:
      glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
      break;
    case CursorMode::Normal:
      glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      break;
    }

    glfwSetKeyCallback(window_, InputHandler::KeyCallback);
    glfwSetCursorPosCallback(window_, InputHandler::MouseCallback);

    LOG("Window created");
  }

  WindowManager::~WindowManager()
  {
    glfwTerminate();
  }

  bool WindowManager::UpdateWindow()
  {
    glfwPollEvents();
    
    return !glfwWindowShouldClose(window_);
  }

  void WindowManager::SwapBuffers()
  {
    glfwSwapBuffers(window_);

    glClearColor(0.2f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    //glClear(GL_COLOR_BUFFER_BIT);
  }
  double WindowManager::GetDeltaTime()
  {
    double currentFrame = glfwGetTime();
    double deltaTime = currentFrame - lastFrameTime_;
    lastFrameTime_ = currentFrame;
    return deltaTime;
  }
}