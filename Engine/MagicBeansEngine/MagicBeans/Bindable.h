#pragma once


#pragma warning(disable: 4244 4458 4127 4702)
#include <sqrat\sqrat.h>
#include <sqrat\sqrat\sqratClass.h>
#pragma warning(default: 4244 4458 4127 4702)
#include <unordered_map>

#include "ClassName.h"


namespace Beans
{
    template<class BindClass>
    class Bindable
    {
    public:
        //Make sure first use is with the name
        static Sqrat::Class<BindClass>& GetSquirrelClass()
        {
            static Sqrat::Class<BindClass> bindClass(Sqrat::DefaultVM::Get(), sName);
            return bindClass;
        }

        static const char* sName;
    };

    template<class BindClass>
    const char* Bindable<BindClass>::sName = BindClass::MetaInfo.Name.c_str();
    //const char* Bindable<BindClass>::sName = ClassName<BindClass>();
}