#include "GameObject.h"

#include "Component.h"
namespace Beans
{
  unsigned long GameObject::instance_count = 0;
  unsigned long GameObject::created_counter = 0;

  Beans::GameObject::GameObject(const std::string & name) : 
    Name(name), ID(created_counter++), markedForDelete_(false)
  {
    ++instance_count;
    components_.reserve(4);
  }

  Beans::GameObject::~GameObject()
  {
    --instance_count;

    for (Component* comp : components_)
    {
      delete comp;
    }
  }

  void GameObject::Delete()
  {
    markedForDelete_ = true;
  }

  bool GameObject::isMarkedForDelete()
  {
    return markedForDelete_;
  }

  unsigned long GameObject::GetActiveObjects()
  {
    return instance_count;
  }

  void GameObject::Swap(GameObject & other)
  {
    Name.swap(other.Name);
    std::swap(markedForDelete_, other.markedForDelete_);
    components_.swap(other.components_);
  }
}