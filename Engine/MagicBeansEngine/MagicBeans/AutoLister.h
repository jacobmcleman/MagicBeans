#pragma once

#include <unordered_set>

#ifndef AUTOLISTER_H
#define AUTOLISTER_H

namespace Beans
{
  namespace Utilities
  {
    template <typename T>
    class AutoLister
    {
    public:
      static const std::unordered_set<T*>& GetList();
    protected:
      AutoLister();
      ~AutoLister();

    private:
      static std::unordered_set<T*> list;
    };

    template<typename T>
    inline const std::unordered_set<T*>& AutoLister<T>::GetList()
    {
      return list;
    }

    template<typename T>
    inline AutoLister<T>::AutoLister()
    {
      //Add this object to the list
      list.insert(static_cast<T*>(this));
    }

    template<typename T>
    inline AutoLister<T>::~AutoLister()
    {
      list.erase(static_cast<T*>(this));
    }

    template<typename T>
    std::unordered_set<T*> AutoLister<T>::list = std::unordered_set<T*>();
  }
}


#endif