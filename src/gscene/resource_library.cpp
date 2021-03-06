#include "stdafx.h"

#include "gscene/resource_library.h"

#include "gthread/job_manager.h"

#include "gserializer/serializer.h"

#include <fmt/format.h>

namespace gscene
{
    void resource_library::process(gserializer::serializer& serializer)
    {
        serializer.process("m_resources", m_resources, resource::factory());
    }

    void resource_library::load(gthread::job_manager& jobs)
    {
        std::vector<std::future<void>> load_handles;
        load_handles.reserve(m_resources.size());

        for (auto const& res_pair : m_resources)
        {
            resource* res = res_pair.second.get();
            load_handles.emplace_back(jobs.submit_with_future([=]() { res->load(); }));
        }

        for (std::future<void>& load_handle : load_handles)
        {
            load_handle.get();
        }
    }
    void resource_library::throw_error_resource_does_not_exist(std::string const& name) const
    {
        throw std::runtime_error(fmt::format("Trying to get resource that doesn't exist: {0}", name));
    }
}

