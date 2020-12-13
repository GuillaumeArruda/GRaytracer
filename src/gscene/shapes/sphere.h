#pragma once
#include "gscene/shape.h"

namespace gscene
{
    struct sphere final : shape
    {
        sphere(float radius)
            : m_radius2(radius * radius) {}
        sphere() noexcept = default;
        sphere(sphere const&) noexcept = default;
        sphere(sphere&&) noexcept = default;
        ~sphere() noexcept = default;

        sphere& operator=(sphere const&) noexcept = default;
        sphere& operator=(sphere&&) noexcept = default;

        void process(gserializer::serializer& serializer);

        std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept final;
        gmath::axis_aligned_box<gmath::world_space> world_bounds(world_transform const& transform) const noexcept final;
    private:
        float m_radius2 = 0.f;
        GSERIALIZER_DECLARE_SUBCLASS_FACTORY_REGISTRATION();
    };
}
