#pragma once

#include <string>

struct GLFWwindow;

namespace Beans
{
  class WindowManager
  {
  public:
    WindowManager(const std::string& title, int width = 800, int height = 600);
    ~WindowManager();

    bool UpdateWindow();
    void SwapBuffers();

  private:
    GLFWwindow* window_;
    int width_;
    int height_;
  };

  
}