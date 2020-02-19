#pragma once

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>

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

        template<typename Archive>
        void serialize(Archive& ar)
        {
            ar(CEREAL_NVP(m_radius2));
        }

    private:
        std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept final;
        gmath::axis_aligned_box<gmath::world_space> world_bounds(world_transform const& transform) const noexcept final;
             
        float m_radius2 = 0.f;
    };
}

CEREAL_REGISTER_TYPE(gscene::sphere);
CEREAL_REGISTER_POLYMORPHIC_RELATION(gscene::shape, gscene::sphere);
