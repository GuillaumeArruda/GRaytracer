#include "stdafx.h"

#include "gmath/vector.h"

#include "gscene/lights/point_light.h"

#include <glm/gtx/compatibility.hpp>

namespace gscene
{
    float point_light::light_ratio_at_position(gmath::position<gmath::world_space> const& position) const
    {
        float const distance = (position - get_transform().get_translation()).length();
        if (distance <= m_fallout_min_distance)
            return 1.f;
        if (distance >= m_fallout_max_distance)
            return 0.f;
        float const lerpFactor = (distance - m_fallout_min_distance) / (m_fallout_max_distance - m_fallout_min_distance);
        return glm::lerp(1.f, 0.f, lerpFactor);
    }
}
