#pragma once
#include "gscene/light.h"

namespace gscene
{
    struct point_light : light
    {
        float light_ratio_at_position(gmath::position<gmath::world_space> const& position) const override;
        void process(gserializer::serializer& serializer) override;
    private:
        float m_fallout_min_distance = 0.f;
        float m_fallout_max_distance = 10.f;

        GSERIALIZER_DECLARE_SUBCLASS_FACTORY_REGISTRATION();
    };
}