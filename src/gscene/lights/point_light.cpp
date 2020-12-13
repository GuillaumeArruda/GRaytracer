#include "stdafx.h"

#include "gmath/vector.h"

#include "gscene/lights/point_light.h"

#include <glm/gtx/compatibility.hpp>

#include "gserializer/serializer.h"
#include "gserializer/gmath_serialization.h"

GSERIALIZER_DEFINE_SUBCLASS_FACTORY_REGISTRATION(gscene::point_light);

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

    void point_light::process(gserializer::serializer& serializer)
    {
        light::process(serializer);
        serializer.process("m_fallout_min_distance", m_fallout_min_distance);
        serializer.process("m_fallout_max_distance", m_fallout_max_distance);
    }
}
