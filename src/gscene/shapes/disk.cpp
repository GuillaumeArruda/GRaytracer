#include "stdafx.h"

#include "gscene/shapes/disk.h"
#include "gscene/resources/material.h"
#include "gscene/object.h"

GSERIALIZER_DEFINE_SUBCLASS_FACTORY_REGISTRATION(gscene::disk);

namespace gscene
{
    void disk::process(gserializer::serializer& serializer)
    {
        serializer.process("m_normal", m_normal);
        serializer.process("radius", m_radius2);
    }

    std::optional<ray_hit> disk::raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept
    {
        float const denom = m_normal.dot(ray.dir());
        if (std::abs(denom) < std::numeric_limits<float>::epsilon())
            return {};

        gmath::position<gmath::world_space> const obj_position = obj.get_transform().get_translation();
        gmath::vector<gmath::world_space> const ray_to_plane = obj_position - ray.start();
        float const t = ray_to_plane.dot(m_normal) / denom;

        if (!ray.is_valid_t(t))
            return {};

        gmath::position<gmath::world_space> const hit_pos = ray(t);
        if ((obj_position - hit_pos).length2() > m_radius2)
            return {};

        return ray_hit{ {hit_pos, m_normal, &ray, t}, &obj };
    }
    gmath::axis_aligned_box<gmath::world_space> disk::world_bounds(world_transform const& transform) const noexcept
    {
        float const radius = std::sqrt(m_radius2);
        gmath::position<gmath::world_space> const position = transform.get_translation();
        gmath::vector<gmath::world_space> const on_plane_direction = m_normal.cross(gmath::vector<gmath::world_space>(-m_normal.y(), m_normal.z(), m_normal.x()));
        return gmath::axis_aligned_box<gmath::world_space>(position - on_plane_direction * radius, position + on_plane_direction * radius);
    }
}