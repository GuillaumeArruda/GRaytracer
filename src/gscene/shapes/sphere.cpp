#include "stdafx.h"

#include "gscene/shapes/sphere.h"
#include "gscene/resources/material.h"
#include "gscene/object.h"

#include "gserializer/serializer.h"
#include "gserializer/gmath_serialization.h"

GSERIALIZER_DEFINE_SUBCLASS_FACTORY_REGISTRATION(gscene::sphere);

namespace gscene
{
    void sphere::process(gserializer::serializer& serializer)
    {
        serializer.process("m_radius2", m_radius2);
    }

    std::optional<ray_hit> sphere::raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept
    {
        const gmath::ray<gmath::model_space> local_ray = obj.get_inverse_transform() * ray;

        float const a = local_ray.dir().length2();
        float const b = 2.f * local_ray.dir().dot(gmath::vector(local_ray.start()));
        float const c = gmath::vector(local_ray.start()).length2() - m_radius2;

        if (std::optional<gmath::quadatric_result> const quad_coefficient = gmath::quadatric(a, b,  c))
        {
            float t;
            if (local_ray.is_valid_t(quad_coefficient->m_first_t))
                t = quad_coefficient->m_first_t;
            else if (local_ray.is_valid_t(quad_coefficient->m_second_t))
                t = quad_coefficient->m_second_t;
            else
                return {};

            gmath::position<gmath::world_space> const hitpos = ray(t);
            gmath::direction<gmath::world_space> const normal = hitpos - obj.get_transform().get_translation();
            return ray_hit{{hitpos, normal, &ray, t}, &obj };
        }
        return {};
    }
    gmath::axis_aligned_box<gmath::world_space> sphere::world_bounds(world_transform const& transform) const noexcept
    {
        float const radius = std::sqrt(m_radius2);
        gmath::vector<gmath::world_space> const radius_vector(radius, radius, radius);
        return gmath::axis_aligned_box<gmath::world_space>(transform.get_translation() - radius_vector, transform.get_translation() + radius_vector);
    }
}


