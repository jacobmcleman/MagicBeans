#pragma once

#include <string>

struct GLFWwindow;

namespace Beans
{
  enum struct CursorMode
  {
    Disabled, Hidden, Normal
  };

  class WindowManager
  {
  public:
    WindowManager(const std::string& title, CursorMode cursor = CursorMode::Normal, int width = 800, int height = 600);
    ~WindowManager();

    bool UpdateWindow();
    void SwapBuffers();

    double GetDeltaTime();
    bool IsKeyTriggered(int key);

  private:
    GLFWwindow* window_;
    int width_;
    int height_;

    double lastFrameTime_;
  };

  
}