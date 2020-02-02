#pragma once

#include "gmath/position.h"
#include "gmath/direction.h"

namespace gscene
{
    struct object;
    struct material;
    struct ray_hit
    {
        gmath::position<gmath::world_space> m_position;
        gmath::direction<gmath::world_space> m_normal;
        object const* m_shape;
        float m_t = 0.f;

        bool operator==(ray_hit const& rhs) const noexcept
        {
            return m_t == rhs.m_t && m_position == rhs.m_position && m_normal == rhs.m_normal && &m_shape == &rhs.m_shape;
        }
        bool operator<(ray_hit const& rhs) const noexcept
        {
            return m_t < rhs.m_t;
        }
    };
}