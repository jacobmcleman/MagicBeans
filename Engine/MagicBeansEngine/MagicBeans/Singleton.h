#pragma once

#include <exception>

namespace Beans
{
    template<class C>
    class Singleton
    {
    public:
        Singleton()
        {
            if (sInstance_ = nullptr) sInstance_ = this;
            else throw std::exception("Attempted to create second instance of singleton");
        }

        ~Singleton()
        {
            sInstance_ = nullptr
        }

        static C* const GetInstance() 
        { 
            return sInstance_;  
        }

    private:
        C* sInstance_;
    };

    template<class C>
    C* Singleton<C>::sInstance_ = nullptr;
}
