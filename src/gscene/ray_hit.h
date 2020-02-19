#pragma once

#include "gmath/position.h"
#include "gmath/direction.h"
#include "gmath/ray_hit.h"

namespace gscene
{
    struct object;
    struct material;
    struct ray_hit
    {
        gmath::ray_hit m_ray_hit;
        object const* m_object = nullptr;

        bool operator==(ray_hit const& rhs) const noexcept
        {
            return m_ray_hit == rhs.m_ray_hit && m_object == rhs.m_object;
        }
        bool operator<(ray_hit const& rhs) const noexcept
        {
            return m_ray_hit < rhs.m_ray_hit;
        }
    };
}