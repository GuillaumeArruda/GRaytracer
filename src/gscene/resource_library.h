#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "gscene/resource.h"

namespace gserializer
{
    struct serializer;
}

namespace gthread
{
    struct job_manager;
}

namespace gscene
{
    struct resource_library
    {
        resource_library() = default;
        ~resource_library() = default;

        resource_library(resource_library const&) = delete;
        resource_library& operator=(resource_library const&) = delete;

        resource_library(resource_library&&) noexcept = default;
        resource_library& operator=(resource_library&&) = default;

        void process(gserializer::serializer& serializer);

        void load(gthread::job_manager& threading_pool);


        template<class ResourceType>
        ResourceType const* get_resource(std::string const& name) const
        {
            auto const it = m_resources.find(name);
            if (it != m_resources.end())
            {
                ResourceType const& ref = dynamic_cast<ResourceType const&>(*it->second.get());
                return &ref;
            }
            throw_error_resource_does_not_exist(name);
            return nullptr;
        }

    private:
        void throw_error_resource_does_not_exist(std::string const& name) const;

        std::unordered_map<std::string, std::unique_ptr<resource>> m_resources;
    };
}