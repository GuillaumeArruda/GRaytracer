#include "gscene/lenses/pinhole_lense.h"

namespace gscene
{
    std::vector<gmath::ray<gmath::world_space>> pinhole_lense::generate_rays(world_transform const& transform, camera::generate_rays_params const& params) const
    {
        std::size_t const number_of_rays = params.number_of_pixel * params.axis_aliasing_sample * params.axis_aliasing_sample;
        std::vector<gmath::ray<gmath::world_space>> rays;
        rays.reserve(number_of_rays);

        float const half_h = -std::tan(0.5f * static_cast<float>(m_fov));
        float const half_w = half_h * (static_cast<float>(params.x_resolution) / params.y_resolution);
        float const inverse_width = 1.f / params.x_resolution;
        float const inverse_height = 1.f / params.y_resolution;
        float const inverse_aliasing = 2.f / params.axis_aliasing_sample;

        gmath::position<gmath::world_space> const origin = transform.get_translation();
        for (std::size_t i = 0; i < params.number_of_pixel; ++i)
        {
            std::size_t const x_index = (params.start_pixel + i) % params.x_resolution;
            std::size_t const y_index = (params.start_pixel + i) / params.x_resolution;
            for (std::size_t k = 0; k < params.axis_aliasing_sample; ++k)
            {
                float const k_sign = (k & 1) == 0 ? 1.f : -1.f;
                for (std::size_t l = 0; l < params.axis_aliasing_sample; ++l)
                {
                    float const l_sign = (l & 1) == 0 ? 1.f : -1.f;
                    gmath::vector<gmath::model_space> const local_direction(
                        (2.f * (x_index + (k * k_sign * inverse_aliasing)) * inverse_width - 1) * half_w,
                        (2.f * (y_index + (l * l_sign * inverse_aliasing)) * inverse_height - 1) * half_h,
                        1.f);
                    rays.emplace_back(origin, (transform * local_direction).normalize());
                }
            }
        }

        return rays;
    }
}

