#pragma once

#include <vector>

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
      static const std::vector<T*>& GetList();
    protected:
      AutoLister();
      ~AutoLister();

    private:
      static std::vector<T*> list;
    };

    template<typename T>
    inline const std::vector<T*>& AutoLister<T>::GetList()
    {
      return list;
    }

    template<typename T>
    inline AutoLister<T>::AutoLister()
    {
      //Add this object to the list
      list.push_back(reinterpret_cast<T*>(this));
    }

    template<typename T>
    inline AutoLister<T>::~AutoLister()
    {
      //Find this object in the list
      for (auto it = list.begin(); it != list.end(); ++it)
      {
        //Remove the object from the list
        if (*it == this)
        {
          iter_swap(it, list.end() - 1);
          list.pop_back();
          break;
        }
      }
    }

    template<typename T>
    std::vector<T*> AutoLister<T>::list = std::vector<T*>();
  }
}


#endif