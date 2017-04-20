#pragma once

namespace Beans
{
  class GameObject;

  class Component
  {
  public:
    Component(GameObject* owner);

    GameObject * const Owner;
  };
}