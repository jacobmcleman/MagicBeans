#pragma once
#include "MetaType.h"
#include "Property.h"
#include "AutoLister.h"

namespace Beans
{
  class GameObject;

  class Component
  {
  public:
    Component(GameObject* owner);
    virtual ~Component() {}

    virtual const Utilities::MetaTypeBase& GetMetaType() const = 0;

    GameObject * const Owner;
  };
}