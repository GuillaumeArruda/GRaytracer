#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include <fmt/format.h>

#include <cereal/types/unordered_map.hpp>

#include "gscene/resource.h"

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

        template<class Archive>
        void serialize(Archive& ar)
        {
            ar(CEREAL_NVP(m_resources));
        }

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
            throw std::runtime_error(fmt::format("Trying to get resource that doesn't exist: {0}", name));
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<resource>> m_resources;
    };
}