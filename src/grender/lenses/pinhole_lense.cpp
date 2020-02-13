#include "stdafx.h"

#include "grender/lenses/pinhole_lense.h"

namespace grender
{
    std::vector<gmath::ray<gmath::world_space>> pinhole_lense::generate_rays(camera const& camera, camera::generate_rays_params const& params) const
    {
        std::size_t const number_of_rays = params.number_of_pixel * params.axis_aliasing_sample * params.axis_aliasing_sample;
        std::vector<gmath::ray<gmath::world_space>> rays;
        rays.reserve(number_of_rays);

        std::size_t const width = camera.get_sensor().get_width();
        std::size_t const height = camera.get_sensor().get_height();
        float const half_h = -std::tan(0.5f * static_cast<float>(m_fov));
        float const half_w = half_h * (static_cast<float>(width) / height);
        float const inverse_width = 1.f / camera.get_sensor().get_width();
        float const inverse_height = 1.f / camera.get_sensor().get_height();
        float const inverse_aliasing = 2.f / params.axis_aliasing_sample;

        gmath::position<gmath::world_space> const origin = camera.get_world_transform().get_translation();
        for (std::size_t i = 0; i < params.number_of_pixel; ++i)
        {
            std::size_t const current_index = params.start_pixel + i;
            std::size_t const y_index = current_index / width;
            std::size_t const x_index = current_index % width;
            for (std::size_t k = 0; k < params.axis_aliasing_sample; ++k)
            {
                float const k_value = (k & 1) == 0 ? static_cast<float>(k) : -static_cast<float>(k);
                float const x_pos = (2.f * (x_index + (k_value * inverse_aliasing)) * inverse_width - 1) * half_w;
                for (std::size_t l = 0; l < params.axis_aliasing_sample; ++l)
                {
                    float const l_value = (l & 1) == 0 ? static_cast<float>(l) : -static_cast<float>(l);
                    float const y_pos = (2.f * (y_index + (l_value * inverse_aliasing)) * inverse_height - 1) * half_h;
                    gmath::vector<gmath::model_space> const local_direction(
                        x_pos,
                        y_pos,
                        1.f);
                    rays.emplace_back(origin, (camera.get_world_transform() * local_direction).normalize());
                }
            }
        }

        return rays;
    }
}

