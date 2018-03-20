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
      static std::unordered_set<T*>& GetListNonConst();
    };

    template<typename T>
    inline std::unordered_set<T*>& AutoLister<T>::GetListNonConst()
    {
      static std::unordered_set<T*> list;
      return list;
    }

    template<typename T>
    inline const std::unordered_set<T*>& AutoLister<T>::GetList()
    {
      return GetListNonConst();
    }

    template<typename T>
    inline AutoLister<T>::AutoLister()
    {
      //Add this object to the list
      GetListNonConst().insert(static_cast<T*>(this));
    }

    template<typename T>
    inline AutoLister<T>::~AutoLister()
    {
      GetListNonConst().erase(static_cast<T*>(this));
    }
  }
}


#endif