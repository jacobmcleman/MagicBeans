#pragma once



struct GLFWwindow;

namespace Beans
{
  class InputHandler
  {
  public:
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
  };
}