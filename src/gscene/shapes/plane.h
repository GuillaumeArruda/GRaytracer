#pragma once
#include "gscene\shape.h"
namespace gscene
{
    struct plane final : shape
    {
        plane(gmath::direction<gmath::world_space> const& normal) noexcept : m_normal(normal) {}
        plane(plane const&) noexcept = default;
        plane(plane&&) noexcept = default;
        ~plane() noexcept = default;

        plane& operator=(plane const&) noexcept = default;
        plane& operator=(plane&&) noexcept = default;

    private:
        gmath::direction<gmath::world_space> m_normal;
        
        std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept final;
    };
}


