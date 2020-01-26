#pragma once

#include "gmath\transform.hpp"
namespace gscene
{
    struct camera
    {
        camera() noexcept = default;
        camera(gmath::transform<gmath::camera_space, gmath::world_space> const& transform) noexcept : m_camera_from_world(transform) {};
        camera(camera const&) noexcept = default;
        camera(camera&&) noexcept = default;
        ~camera() noexcept = default;

        camera& operator=(camera const&) noexcept = default;
        camera& operator=(camera&&) noexcept = default;


    private:
        gmath::transform<gmath::camera_space, gmath::world_space> m_camera_from_world;
    };
}