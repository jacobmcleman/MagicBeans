#include "InputHandler.h"

//GLFW
#include <GLFW\glfw3.h>

unsigned int Beans::InputHandler::keymap[] = { 0 };
double Beans::InputHandler::mouse_lastX = 400;
double Beans::InputHandler::mouse_lastY = 300;
double Beans::InputHandler::mouse_xMotion = 0;
double Beans::InputHandler::mouse_yMotion = 0;
double Beans::InputHandler::mouse_sensitivity = 0.05;

void Beans::InputHandler::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
  (void)window;
  (void)scancode;
  (void)mods;

  //Just...don't
  if (key > numKeys_) return;

  if (action == GLFW_PRESS)
  {
    keymap[key / keysPerChar_] |= (1 << ((key % keysPerChar_) * bytesPerKey_));
    keymap[key / keysPerChar_] |= (1 << (((key % keysPerChar_) * bytesPerKey_) + 1));
  }
  else if (action == GLFW_RELEASE)
  {
    keymap[key / keysPerChar_] ^= (1 << (key % keysPerChar_) * bytesPerKey_);
  }
}

void Beans::InputHandler::MouseCallback(GLFWwindow * window, double xpos, double ypos)
{
  (void)window;

  double xChange = xpos - mouse_lastX;
  double yChange = -(ypos - mouse_lastY); // negative because window coords are upside down

  mouse_lastX = xpos;
  mouse_lastY = ypos;

  mouse_xMotion = mouse_sensitivity * xChange;
  mouse_yMotion = mouse_sensitivity * yChange;
}

void Beans::InputHandler::UpdateInput()
{
  for (int i = 0; i < numChars_; i++)
  {
    //Zero out the triggered bits before starting frame
    keymap[i] &= 0b01010101010101010101010101010101;
  }
}

bool Beans::InputHandler::IsKeyDown(int keycode)
{
  return (keymap[keycode / keysPerChar_] & (1 << ((keycode % keysPerChar_) * bytesPerKey_))) != 0;;
}

bool Beans::InputHandler::IsKeyTriggered(int keycode)
{
  return ((keymap[keycode / keysPerChar_]) & (1 << (((keycode % keysPerChar_) * bytesPerKey_) + 1))) != 0;
}

vec2 Beans::InputHandler::GetMouseMotion()
{
  return vec2((float)mouse_xMotion, (float)mouse_yMotion);
}

void Beans::InputHandler::SetMouseSensitivity(double val)
{
  mouse_sensitivity = val;
}
