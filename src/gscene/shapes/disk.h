#pragma once

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

        void process(gserializer::serializer& serializer) override;

        std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept final;
        gmath::axis_aligned_box<gmath::world_space> world_bounds(world_transform const& transform) const noexcept final;
    private:
        gmath::direction<gmath::world_space> m_normal;
        float m_radius2 = 1.f;
        GSERIALIZER_DECLARE_SUBCLASS_FACTORY_REGISTRATION();
    };
}