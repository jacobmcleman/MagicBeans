#pragma once

#include <string>
#include <unordered_map>

namespace Beans
{

    typedef unsigned long long id_t;

    class GenericProperty;

    namespace Utilities
    {
#define REFLECT_CLASS(classname) static Beans::Utilities::MetaType<classname> MetaInfo;\
                      virtual const Beans::Utilities::MetaType<classname>& GetMetaType() const

#define INIT_REFLECTION(classname) Beans::Utilities::MetaType<classname> classname##::MetaInfo = Beans::Utilities::MetaType<classname>( #classname );\
                                   const Beans::Utilities::MetaType<classname>& classname##::GetMetaType() const { return MetaInfo; }

        struct MetaTypeBase
        {
            MetaTypeBase(const char* name) : ID(idCounter++), Name(name) {}
            const id_t ID;
            const std::string Name;

        private:
            static id_t idCounter;
        };

        template<class C>
        struct MetaType : public MetaTypeBase
        {
            MetaType(const char* name) : MetaTypeBase(name) {}

            void RegisterProperty(const std::string& aName, GenericProperty C::* aProperty)
            {
                propertiesMap_[aName] = aProperty;
            }

            const std::unordered_map<std::string, GenericProperty C::*>& GetPropertyMap() const
            {
                return propertiesMap_;
            }

        private:
            std::unordered_map<std::string, GenericProperty C::*> propertiesMap_;
        };
    }
}