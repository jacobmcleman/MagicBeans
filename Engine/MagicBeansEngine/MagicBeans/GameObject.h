#pragma once

#include <string>
#include <vector>



namespace Beans
{
  class Component;

  class GameObject
  {
  public:
    GameObject(const std::string& name);
    ~GameObject();

    template<typename T>
    T* AddComponent();

    template<typename T>
    T* GetComponent();

    template<typename T>
    void RemoveComponent();

    std::string Name;
    const unsigned long ID;

    void Delete();

    bool isMarkedForDelete();

    static unsigned long GetActiveObjects();

    void Swap(GameObject& other);

  private:
    std::vector<Component*> components_;
    static unsigned long created_counter;
    static unsigned long instance_count;
    bool markedForDelete_;
  };


  template<typename T>
  inline T * GameObject::AddComponent()
  {
    if (components_.empty())
    {
      T* newComp = new T(this);
      components_.push_back(newComp);
      return newComp;
    }
    else
    {
      size_t min = 0;
      size_t max = components_.size();
      size_t mid;

      //The ID of the component type to search for
      unsigned int searchID = T::MetaInfo.ID;

      //Binary search for component type matching ID in SORTED component list
      while (min < max)
      {
        mid = (min + max) / 2;
        unsigned int midID = components_[mid]->GetMetaType().ID;

        if (midID == searchID)
        {
          return static_cast<T*>(components_[mid]);
        }
        else if (midID < searchID)
        {
          min = mid + 1;
        }
        else if (midID > searchID)
        {
          max = mid;
        }
      }


      //Create the component
      T* newComp = new T(this);

      if (min < components_.size())
      {
        //Find where to insert at based on the end state of the binary search
        auto insertAt = components_.begin() + (min);

        //Add the component at that position
        components_.insert(insertAt, newComp);
      }
      else
      {
        //Add the component at the end
        components_.insert(components_.end(), newComp);
      }

      //Give the component back
      return newComp;
    }
  }
  template<typename T>
  inline T * GameObject::GetComponent()
  {
    size_t min = 0;
    size_t max = components_.size();
    size_t mid;

    //The ID of the component type to search for
    unsigned int searchID = T::MetaInfo.ID;

    //Binary search the component vector for the type matching the ID
    while (min < max)
    {
      mid = (min + max) / 2;
      unsigned int midID = components_[mid]->GetMetaType().ID;

      if (midID == searchID)
      {
        return reinterpret_cast<T*>(components_[mid]);
      }
      else if (midID < searchID)
      {
        min = mid + 1;
      }
      else if (midID > searchID)
      {
        max = mid;
      }
    }

    return nullptr;
  }
  template<typename T>
  inline void GameObject::RemoveComponent()
  {
    size_t min = 0;
    size_t max = components_.size();
    size_t mid;

    //The ID of the component type to search for
    unsigned int searchID = T::MetaInfo.ID;

    //Binary search for component type matching ID in SORTED component list
    while (min < max)
    {
      mid = (min + max) / 2;
      unsigned int midID = components_[mid]->GetMetaType().ID;

      if (midID == searchID)
      {
        auto removeAt = components_.begin() + (min + 1);
        components_.erase(removeAt);
        return;
      }
      else if (midID < searchID)
      {
        min = mid + 1;
      }
      else if (midID > searchID)
      {
        max = mid;
      }
    }
  }
}