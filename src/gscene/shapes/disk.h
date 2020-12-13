#pragma once

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>

#include "gscene/shape.h"

namespace gscene
{
    struct disk final : shape
    {
        disk(gmath::direction<gmath::world_space> const& normal, float radius) noexcept : m_normal(normal), m_radius2(radius * radius) {}
        disk() noexcept = default;
        disk(disk const&) noexcept = default;
        disk(disk&&) noexcept = default;
        ~disk() noexcept = default;

        disk& operator=(disk const&) noexcept = default;
        disk& operator=(disk&&) noexcept = default;

        template<typename Archive>
        void serialize(Archive & ar)
        {
            float radius = std::sqrt(m_radius2);
            ar(CEREAL_NVP(m_normal), CEREAL_NVP(radius));
            m_radius2 = radius * radius;
        }

        void process(gserializer::serializer& serializer) override;

        std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept final;
        gmath::axis_aligned_box<gmath::world_space> world_bounds(world_transform const& transform) const noexcept final;
    private:
        gmath::direction<gmath::world_space> m_normal;
        float m_radius2 = 1.f;
        GSERIALIZER_DECLARE_SUBCLASS_FACTORY_REGISTRATION();
    };
}

CEREAL_REGISTER_TYPE(gscene::disk);
CEREAL_REGISTER_POLYMORPHIC_RELATION(gscene::shape, gscene::disk);