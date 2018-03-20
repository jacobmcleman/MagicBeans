#pragma once

#include <chrono>
#include <string>

struct SDL_Window;

namespace Beans
{
  enum struct CursorMode
  {
    Disabled, Hidden, Normal
  };

  class WindowManager
  {
  public:
    typedef std::chrono::time_point<std::chrono::system_clock> time_point;

    WindowManager(const std::string& title, CursorMode cursor = CursorMode::Normal, int width = 800, int height = 600);
    ~WindowManager();

    bool UpdateWindow();
    void SwapBuffers();

    double GetDeltaTime();

  private:
    SDL_Window* window_;
    int width_;
    int height_;

    time_point lastFrameTime_;
  };

  
}