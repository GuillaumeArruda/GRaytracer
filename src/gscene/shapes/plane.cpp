#include "stdafx.h"

#include "gscene/shapes/plane.h"
#include "gscene/resources/material.h"
#include "gscene/object.h"

#include "gserializer/serializer.h"
#include "gserializer/gmath_serialization.h"

GSERIALIZER_DEFINE_SUBCLASS_FACTORY_REGISTRATION(gscene::plane);

namespace gscene
{
    void plane::process(gserializer::serializer& serializer)
    {
        serializer.process("m_normal", m_normal);
    }

    std::optional<ray_hit> plane::raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept
    {
        float const denom = m_normal.dot(ray.dir());
        if (std::abs(denom) < std::numeric_limits<float>::epsilon())
            return {};

        gmath::vector<gmath::world_space> const ray_to_plane = obj.get_transform().get_translation() - ray.start();
        float const t = ray_to_plane.dot(m_normal) / denom;

        if (!ray.is_valid_t(t))
            return {};

        return ray_hit{ {ray(t), m_normal, &ray, t},&obj };
    }
    gmath::axis_aligned_box<gmath::world_space> plane::world_bounds(world_transform const& transform) const noexcept
    {
        float constexpr extend = 10000.f;
        gmath::position<gmath::world_space> const position = transform.get_translation();
        gmath::vector<gmath::world_space> const on_plane_direction = m_normal.cross(gmath::vector<gmath::world_space>(-m_normal.y(), m_normal.z(), m_normal.x()));
        return gmath::axis_aligned_box<gmath::world_space>(position - on_plane_direction * extend, position + on_plane_direction * extend);
    }
}
