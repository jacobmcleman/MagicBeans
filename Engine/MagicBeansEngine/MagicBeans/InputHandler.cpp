#include "InputHandler.h"

//GLFW
#include <GLFW\glfw3.h>

unsigned int Beans::InputHandler::keymap[] = { 0 };

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
