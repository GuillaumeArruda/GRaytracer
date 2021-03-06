#pragma once
#include "gscene/shape.h"

namespace gscene
{
    struct plane final : shape
    {
        plane(gmath::direction<gmath::world_space> const& normal) noexcept : m_normal(normal) {}
        plane() noexcept = default;
        plane(plane const&) noexcept = default;
        plane(plane&&) noexcept = default;
        ~plane() noexcept = default;

        plane& operator=(plane const&) noexcept = default;
        plane& operator=(plane&&) noexcept = default;

        void process(gserializer::serializer& serializer) override;

        std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept final;
        gmath::axis_aligned_box<gmath::world_space> world_bounds(world_transform const& transform) const noexcept final;
   
    private:
        gmath::direction<gmath::world_space> m_normal;
        GSERIALIZER_DECLARE_SUBCLASS_FACTORY_REGISTRATION();
    };
}