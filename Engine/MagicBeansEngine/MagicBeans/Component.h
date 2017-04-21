#pragma once
#include "MetaType.h"
#include "Property.h"

namespace Beans
{
  class GameObject;

  class Component
  {
  public:
    Component(GameObject* owner);

    virtual const Utilities::MetaType& GetMetaType() const = 0;

    GameObject * const Owner;
  };
}