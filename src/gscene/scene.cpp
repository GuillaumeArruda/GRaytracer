#include "stdafx.h"

#include <glm/gtc/matrix_transform.hpp>

#include "gscene/scene.h"
#include "gscene/light.h"
#include "gscene/shape.h"
#include "gscene/object.h"

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

        std::vector<object> new_objs;
        for (object const& obj : m_objects)
        {
            obj.subdivide(new_objs);
        }

        m_objects.reserve(m_objects.size() + new_objs.size());
        for (object& obj : new_objs)
        {
            m_objects.push_back(std::move(obj));
        }

        m_accelerator->build(*this);
    }
    void scene::process(gserializer::serializer& serializer)
    {
        serializer.process("m_objects", m_objects);
        serializer.process("m_lights", m_lights, light::factory());
        serializer.process("m_accelerator", m_accelerator, accelerator::factory());
    }
}
