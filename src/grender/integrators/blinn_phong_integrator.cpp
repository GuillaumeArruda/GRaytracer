#include "grender\integrators\blinn_phong_integrator.h"

#include "gscene\scene.h"
#include "gscene\light.h"
#include "gscene\material.h"

#include "grender\camera.h"

namespace grender
{
    void blinn_phong_integrator::render(gscene::scene const& scene, grender::camera const& camera, glm::uvec2 bounds) const
    {
        std::size_t constexpr nb_aliasing_iteration = 4;
        std::size_t constexpr number_of_ray_per_pixel = nb_aliasing_iteration * nb_aliasing_iteration;
        float constexpr raycolor_contribution = 1.f / number_of_ray_per_pixel;
        grender::camera::generate_rays_params const params
        {
            bounds.x,                                                   // start_pixel
            bounds.y - bounds.x,                                        // number_of_pixel
            nb_aliasing_iteration,                                      // axis_aliasing_sample
        };
        std::vector<gmath::ray<gmath::world_space>> const rays = camera.generate_rays(params);
        for (std::size_t i = 0; i < params.number_of_pixel; ++i)
        {
            std::size_t const base_index = i * number_of_ray_per_pixel;
            std::size_t const current_index = params.start_pixel + i;
            std::size_t const y_index = current_index / camera.get_sensor().get_width();
            std::size_t const x_index = current_index % camera.get_sensor().get_width();
            for (std::size_t j = 0; j < number_of_ray_per_pixel; ++j)
            {
                if (std::optional<gscene::ray_hit> hit = scene.raycast(rays[base_index + j]))
                {
                    for (auto const& light : scene.get_lights())
                    {
                        glm::vec3 const radiance = compute_radiance(*hit, *light, scene, camera) * raycolor_contribution;
                        camera.get_sensor().set_pixel(x_index, y_index,
                            radiance.r,
                            radiance.g,
                            radiance.b);
                    }
                }
            }
        }
    }

    glm::vec3 blinn_phong_integrator::compute_radiance(gscene::ray_hit const& hit, gscene::light const& light, gscene::scene const& scene, grender::camera const& camera) const noexcept
    {
        gmath::vector<gmath::world_space> const world_light_dir = light.get_transform().get_translation() - hit.m_position;
        float const max_t = world_light_dir.length();
        gmath::ray<gmath::world_space> const ray_to_light(hit.m_position + gmath::vector(hit.m_normal) * 0.0001f, world_light_dir, std::numeric_limits<float>::epsilon(), max_t, 0);
        constexpr glm::vec3 ambient_light(0.05f, 0.05f, 0.05f);
        glm::vec3 const ambient_color = ambient_light * hit.m_object->get_material().m_color;
        if (scene.raycast(ray_to_light))
        {
            return ambient_color;
        }

        gmath::direction<gmath::camera_space> const normal = camera.get_camera_transform() * hit.m_normal;
        gmath::direction<gmath::camera_space> const light_dir = camera.get_camera_transform() * world_light_dir;
        float const lambertian = std::max(light_dir.dot(normal), 0.f);
        float specular_coefficient = 0.f;
        if (lambertian > 0.f)
        {
            gmath::direction<gmath::camera_space> const view_dir = -gmath::vector(camera.get_camera_transform() * hit.m_position);
            gmath::direction<gmath::camera_space> const reflect_dir = -(light_dir).reflect(normal);
            float const specular_angle = std::max(view_dir.dot(reflect_dir), 0.f);
            specular_coefficient = std::pow(specular_angle, 4.f) * lambertian;
        }

        glm::vec3 const diffuse_color = hit.m_object->get_material().m_color * lambertian;
        glm::vec3 const specular_color = hit.m_object->get_material().m_color * specular_coefficient;
        
        return glm::clamp(diffuse_color + specular_color + ambient_color, 0.f, 1.f);
    }
}