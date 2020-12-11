#pragma once

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>


#include "gscene/light.h"

namespace gscene
{
    struct point_light : light
    {
        template<typename Archive>
        void serialize(Archive& ar)
        {
            light::serialize(ar);
            ar(CEREAL_NVP(m_fallout_min_distance), CEREAL_NVP(m_fallout_max_distance));
        }

        float light_ratio_at_position(gmath::position<gmath::world_space> const& position) const override;

    private:
        float m_fallout_min_distance;
        float m_fallout_max_distance;
    };
}


CEREAL_REGISTER_TYPE(gscene::point_light);
CEREAL_REGISTER_POLYMORPHIC_RELATION(gscene::light, gscene::point_light);