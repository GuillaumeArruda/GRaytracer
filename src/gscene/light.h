#pragma once

#include "gscene/common.h"

namespace gscene
{
    struct light
    {
        light(world_transform const& transform) : m_transform(transform) {}
        light() noexcept = default;
        virtual ~light() = 0 {};

        world_transform const& get_transform() const { return m_transform; }
        glm::vec3 const& get_color() const { return m_color; }

        virtual float light_ratio_at_position(gmath::position<gmath::world_space> const& position) const = 0;

        template<typename Archive>
        void serialize(Archive& ar)
        {
            ar(CEREAL_NVP(m_transform), CEREAL_NVP(m_color));
        }

    private:
        world_transform m_transform;
        glm::vec3 m_color;
    };
}