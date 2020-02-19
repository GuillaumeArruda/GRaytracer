#include "stdafx.h"

#include "gscene/shapes/disk.h"
#include "gscene/material.h"
#include "gscene/object.h"


namespace gscene
{
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
        return gmath::axis_aligned_box<gmath::world_space>(transform.get_translation(), transform.get_translation());
    }
}