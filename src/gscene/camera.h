#pragma once

#include <memory>
#include <vector>

#include "gscene/common.h"

#include "gmath/ray.h"

namespace gscene
{
    struct lense;
    struct camera
    {
        struct generate_rays_params
        {
            std::size_t start_pixel = 0;
            std::size_t number_of_pixel = 1920 * 1080;
            std::size_t x_resolution = 1920;
            std::size_t y_resolution = 1080;
            std::size_t axis_aliasing_sample = 1;
        };

        camera(world_transform const& transform, std::unique_ptr<lense> lense) noexcept;
        camera(camera const&) = delete;
        camera(camera&&) noexcept = default;
        ~camera() = default;

        camera& operator=(camera const&) = delete;
        camera& operator=(camera&&) noexcept = default;

        std::vector<gmath::ray<gmath::world_space>> generate_rays(camera::generate_rays_params const& params) const;
    private:
        camera_transform m_camera_transform;
        world_transform m_world_transform;
        std::unique_ptr<lense> m_lense;
    };
}