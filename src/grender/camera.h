#pragma once

#include <memory>
#include <vector>

#include "gmath/ray.h"

#include "gscene/common.h"

#include "grender/sensor.h"

namespace grender
{
    struct lense;
    struct camera
    {
        struct generate_rays_params
        {
            std::size_t start_pixel = 0;
            std::size_t number_of_pixel = 0;
            std::size_t axis_aliasing_sample = 1;
        };

        camera(gscene::world_transform const& transform, std::unique_ptr<lense> lense, std::size_t width, std::size_t height) noexcept;
        camera(camera const&) = delete;
        camera(camera&&) noexcept = default;
        ~camera() = default;

        camera& operator=(camera const&) = delete;
        camera& operator=(camera&&) noexcept = default;

        std::vector<gmath::ray<gmath::world_space>> generate_rays(camera::generate_rays_params const& params) const;

        sensor& get_sensor() const noexcept { return m_sensor; }
        gscene::world_transform const& get_world_transform() const noexcept { return m_world_transform; }
        gscene::camera_transform const& get_camera_transform() const noexcept { return m_camera_transform; }

    private:
        gscene::camera_transform m_camera_transform;
        gscene::world_transform m_world_transform;
        std::unique_ptr<lense> m_lense;
        mutable sensor m_sensor;
    };
}