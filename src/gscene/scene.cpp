#include "stdafx.h"

#include <glm/gtc/matrix_transform.hpp>

#include "gscene/scene.h"
#include "gscene/light.h"


namespace gscene
{
    scene::~scene() = default;

    std::optional<ray_hit> scene::raycast(gmath::ray<gmath::world_space> const& ray) const noexcept
    {
        return m_accelerator->raycast(ray);
    }

    void scene::prepare_scene(gthread::job_manager& jobs)
    {
        m_resource_library.load(jobs);
        for (object& obj : m_objects)
        {
            obj.resolve_resources(m_resource_library);
        }
        m_accelerator->build(*this);
    }
}
