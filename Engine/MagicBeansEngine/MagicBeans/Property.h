#pragma once
#include <functional>

namespace Beans
{
  template<typename T>
  class Property
  {
  public:
    //Set functions take in the given value and then return what the value should be set to
    typedef std::function<T(const T&)> SetFunction;
    //Get functions take the current value and return the value the user should recieve
    typedef std::function<T(const T&)> GetFunction;

    Property(const T& initialValue, SetFunction set = nullptr, GetFunction get = nullptr) :
      data(initialValue), setFunc(set), getFunc(get)
    {

    }

    T Get()
    {
      if (getFunc != nullptr)
      {
        return getFunc(data);
      }
      else
      {
        return data;
      }
    }

    void Set(const T& value)
    {
      if (setFunc != nullptr)
      {
        data = setFunc(value);
      }
      else
      {
        data = value;
      }
    }

    const T& Data() const
    {
      return data;
    }

    operator T()
    {
      return Get();
    }

    const Property& operator=(const T& rhs)
    {
      Set(rhs);
      return *this;
    }

    const Property& operator+=(const T& rhs)
    {
      data += rhs;
      return *this;
    }

    const Property& operator-=(const T& rhs)
    {
      data -= rhs;
      return *this;
    }

    T operator->()
    {
      return data;
    }

  private:
    SetFunction setFunc;
    GetFunction getFunc;
    T data;
  };
}