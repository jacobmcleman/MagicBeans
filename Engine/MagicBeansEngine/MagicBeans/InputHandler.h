#pragma once



struct GLFWwindow;

namespace Beans
{
  class InputHandler
  {
  public:
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void UpdateInput();

    static bool IsKeyDown(int keycode);
    static bool IsKeyTriggered(int keycode);

  private:
    //Number of keys that need to be tracked
    static constexpr const unsigned int numKeys_ = 348;
    //Number of bytes each key needs to store relevant data
    static constexpr const unsigned int bytesPerKey_ = 2;
    //Number of keys that can have their data stored in a single char
    static constexpr const unsigned int keysPerChar_ = ((sizeof(int) * 8) / bytesPerKey_);
    //Number of characters that need to be in the array to hold enough character data
    static constexpr const unsigned int numChars_ = (numKeys_ / keysPerChar_) + 1;

    /*
    Using SDL key scancodes as lookup values

    Characters are just a speedy indexing solution
    Each key has 2 bits
    - First bit is if the key is currently down
    - Second bit is if this is the first frame for which that key has been down
    */
    static unsigned int keymap[numChars_];
  };
}