#pragma once
#include <functional>

#include "Bindable.h"

namespace Beans
{
    class GenericProperty
    {
    public:

    };

    template<class Owner, typename T>
    class Property : public GenericProperty
    {
    public:
        //Set functions take in the given value and then return what the value was set to
        typedef const T&(Owner::* SetFunction)(const T&);
        //Get functions return the value in the property right now
        typedef T(Owner::* GetFunction)() const;

        //Contained Value Constructor
        Property(Owner* aOwner, const char* aName, Property Owner::*classPos, const T& initialValue = T()) :
            owner(aOwner), data(initialValue), setFunc(nullptr), getFunc(nullptr), name(aName)
        {
            owner->GetSquirrelClass().Var(aName, static_cast<T Owner::*>(classPos));

            Owner::MetaInfo.RegisterProperty(aName, reinterpret_cast<GenericProperty Owner::*>(classPos));
        }

        //Contained Value Constructor
        Property(Owner* aOwner, const char* aName, Property Owner::*classPos, GetFunction get, SetFunction set = nullptr, const T& initialValue = T()) :
            owner(aOwner), data(initialValue), setFunc(set), getFunc(get), name(aName)
        {
            Owner::MetaInfo.RegisterProperty(aName, reinterpret_cast<GenericProperty Owner::*>(classPos));

            if (setFunc)
            {
                owner->GetSquirrelClass().Prop(aName, getFunc, setFunc);
                (owner->*setFunc)(initialValue);
            }
            else
            {
                owner->GetSquirrelClass().Prop(aName, getFunc);
            }
        }


        T Get() const
        {
            if (getFunc != nullptr)
            {
                return (owner->*getFunc)();
            }
            else
            {
                return data;
            }
        }

        const T& Set(const T& value)
        {
            if (setFunc != nullptr)
            {
                return (owner->*setFunc)(value);
            }
            else
            {
                data = value;
                return data;
            }
        }

        operator T() const
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
            Set(Get() + rhs);
            return *this;
        }

        const Property& operator-=(const T& rhs)
        {
            Set(Get() - rhs);
            return *this;
        }

        const T operator+(const T& rhs) const
        {
            return Get() + rhs;
        }

        T operator+(const Property& rhs)
        {
            return Get() + rhs.Get();
        }

        T operator-(const Property& rhs)
        {
            return Get() - rhs.Data();
        }

        T operator->()
        {
            return Get();
        }

    private:
        T data;
        SetFunction setFunc;
        GetFunction getFunc;
        Owner* owner;
        std::string name;
    };

    template<typename T, class S>
    const T operator+(const T& rhs, const Property<S, T> lhs)
    {
        return lhs + rhs;
    }
}