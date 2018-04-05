#pragma once
#include <unordered_map>
#include <unordered_set>

#include "Singleton.h"

namespace Beans
{
    class Resource
    {
    public:
        explicit Resource(const std::string& name);
        virtual ~Resource();

        virtual void Load() = 0;
        virtual void Unload() = 0;

        bool GetLoaded() const;

    protected:
        void SetLoaded(bool isLoaded);
    
    private:
        bool isLoaded_;
        const std::string name_;
    };

    class ResourceManager : public Singleton<ResourceManager>
    {
    public:
        explicit ResourceManager(const std::string& baseDirectory);

        Resource* GetResource(const std::string& name);

        template<typename ResourceType>
        ResourceType* GetResource(const std::string& name)
        {
            return reinterpret_cast<ResourceType*>(GetResource(name));
        }

    private:
        std::unordered_map<std::string, Resource*> resources_;
        std::string baseDirectory_;
    };
}