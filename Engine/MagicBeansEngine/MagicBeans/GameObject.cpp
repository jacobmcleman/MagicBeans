#include "GameObject.h"

namespace Beans
{
  unsigned long GameObject::instance_count = 0;
  unsigned long GameObject::created_counter = 0;

  Beans::GameObject::GameObject(const std::string & name) : Name(name), ID(created_counter++)
  {
    ++instance_count;
    components_.reserve(4);
  }

  Beans::GameObject::~GameObject()
  {
    --instance_count;
  }
  unsigned long GameObject::GetActiveObjects()
  {
    return instance_count;
  }
}