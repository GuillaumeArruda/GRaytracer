#include "stdafx.h"

#include "gscene/shapes/plane.h"
#include "gscene/material.h"
#include "gscene/object.h"


namespace gscene
{
    std::optional<ray_hit> plane::raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept
    {
        float const denom = m_normal.dot(ray.dir());
        if (std::abs(denom) < std::numeric_limits<float>::epsilon())
            return {};

        gmath::vector<gmath::world_space> const ray_to_plane = obj.get_transform().get_translation() - ray.start();
        float const t = ray_to_plane.dot(m_normal) / denom;

        if (!ray.is_valid_t(t))
            return {};

        return ray_hit{ ray(t), m_normal, &obj, &ray, t };
    }
}
