#pragma once

#include <string>

namespace Beans
{
  namespace Utilities
  {
#define REFLECT_CLASS const static Beans::Utilities::MetaType MetaInfo;\
                      virtual const Beans::Utilities::MetaType& GetMetaType() const

#define INIT_REFLECTION(classname) const Beans::Utilities::MetaType classname##::MetaInfo = Beans::Utilities::MetaType( #classname );\
                                   const Beans::Utilities::MetaType& classname##::GetMetaType() const { return MetaInfo; }

    struct MetaType
    {
      MetaType(const char* name) : ID(idCounter++), Name(name) {}
      const unsigned long ID;
      const std::string Name;

    private:
      static unsigned long idCounter;
    };
  }
}