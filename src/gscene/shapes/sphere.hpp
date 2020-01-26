#pragma once

#include "gscene\shape.hpp"

namespace gscene
{
    struct sphere final : shape
    {
        sphere(float radius)
            : m_radius(radius) {}
        sphere(sphere const&) noexcept = default;
        sphere(sphere&&) noexcept = default;
        ~sphere() noexcept = default;

        sphere& operator=(sphere const&) noexcept = default;
        sphere& operator=(sphere&&) noexcept = default;

    private:
        std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept final;
             
        float m_radius;
    };
}