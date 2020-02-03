#pragma once

#include "gscene\lense.h"
#include "gmath\units.h"
namespace gscene
{
    struct pinhole_lense : lense
    {
        pinhole_lense(gmath::radian fov, float near_plane, float far_plane) noexcept
            : m_fov(fov), m_near(near_plane), m_far(far_plane) {}
        pinhole_lense(pinhole_lense const&) noexcept = default;
        pinhole_lense(pinhole_lense&&) noexcept = default;
        ~pinhole_lense() noexcept = default;

        pinhole_lense& operator=(pinhole_lense const&) noexcept = default;
        pinhole_lense& operator=(pinhole_lense&&) noexcept = default;

        std::vector<gmath::ray<gmath::world_space>> generate_rays(world_transform const& transform, camera::generate_rays_params const& params) const override;
    
    private:
        gmath::radian m_fov = 70.0_d;
        float m_near = std::numeric_limits<float>::epsilon();
        float m_far = std::numeric_limits<float>::max();
    };
}