#include "InputHandler.h"

//GLFW
#include <GLFW\glfw3.h>

void Beans::InputHandler::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
  (void)mode;
  (void)scancode;

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}
