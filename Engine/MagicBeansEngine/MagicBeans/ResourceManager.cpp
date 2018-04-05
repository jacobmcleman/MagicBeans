#include "ResourceManager.h"

namespace Beans
{
    ResourceManager::ResourceManager(const std::string& baseDirectory) :
        baseDirectory_(baseDirectory)
    {
    }

    Resource* ResourceManager::GetResource(const std::string& name)
    {
        return resources_[name];
    }

    Resource::Resource(const std::string& name) :
        name_(name)
    {
    }

    Resource::~Resource()
    {
    }

    bool Resource::GetLoaded() const
    {
        return isLoaded_;
    }

    void Resource::SetLoaded(bool isLoaded)
    {
        isLoaded_ = isLoaded;
    }

}