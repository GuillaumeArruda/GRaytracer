#include "stdafx.h"

#include "grender/integrators/blinn_phong_integrator.h"

#include "gtl/cast.h"

#include "gscene/scene.h"
#include "gscene/light.h"
#include "gscene/materials/blinn_phong_material.h"

#include "grender/camera.h"

namespace grender
{
    void blinn_phong_integrator::verify_scene(gscene::scene const& scene) const
    {
        for (gscene::object const& object : scene.get_objects())
        {
            if (!dynamic_cast<gscene::blinn_phong_material const*>(&object.get_material()))
            {
                throw std::exception("blinn_phong_integrator require all object to have a blinn_phong_material");
            }
        }
    }
    void blinn_phong_integrator::render(gscene::scene const& scene, grender::camera const& camera, glm::uvec2 bounds) const
    {
        std::size_t constexpr nb_aliasing_iteration = 2;
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
            glm::vec3 radiance = glm::vec3(0.f);
            std::size_t const base_index = i * number_of_ray_per_pixel;
            std::size_t const current_index = params.start_pixel + i;
            std::size_t const y_index = current_index / camera.get_sensor().get_width();
            std::size_t const x_index = current_index % camera.get_sensor().get_width();
            for (std::size_t j = 0; j < number_of_ray_per_pixel; ++j)
            {
                radiance += compute_radiance(rays[base_index + j], scene, camera) * raycolor_contribution;
            }
            radiance = glm::clamp(radiance, 0.f, 1.f);
            camera.get_sensor().set_pixel(x_index, y_index,
                radiance.r,
                radiance.g,
                radiance.b);
        }
    }

    glm::vec3 blinn_phong_integrator::compute_radiance(gmath::ray<gmath::world_space> const& ray, gscene::scene const& scene, grender::camera const& camera) const noexcept
    {
        glm::vec3 color = glm::vec3(0.f);
        if (std::optional<gscene::ray_hit> hit = scene.raycast(ray))
        {
            for (auto const& light : scene.get_lights())
            {
                color += compute_blinn_phong(*hit, *light, scene, camera);
            }

            gscene::blinn_phong_material const& material = gtl::cast<gscene::blinn_phong_material const&>(hit->m_object->get_material());
            if (hit->m_ray_hit.m_ray->depth() < 5 && material.m_reflection_coefficient > 0.f)
            {
                gmath::direction<gmath::world_space> const new_dir = hit->m_ray_hit.m_ray->dir().reflect(hit->m_ray_hit.m_normal);
                gmath::ray<gmath::world_space> const reflection_ray(hit->m_ray_hit.m_position + gmath::vector(hit->m_ray_hit.m_normal) * 0.0001f, new_dir, std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::max(), hit->m_ray_hit.m_ray->depth() + 1);
                color += compute_radiance(reflection_ray, scene, camera) * material.m_reflection_coefficient;
            }
        }
        else
        {
            color = glm::vec3(0.f, 0.f, 0.f);
        }
       

        
        return color;
    }
    
    glm::vec3 blinn_phong_integrator::compute_blinn_phong(gscene::ray_hit const& hit, gscene::light const& light, gscene::scene const& scene, grender::camera const& camera) const noexcept
    {
        gmath::vector<gmath::world_space> const world_light_vec = light.get_transform().get_translation() - hit.m_ray_hit.m_position;
        float const max_t = world_light_vec.length();
        gmath::direction<gmath::world_space> const world_light_dir(gmath::direction<gmath::world_space>::garantee_normal_t::garantee_normal, static_cast<glm::vec3>(world_light_vec / max_t));
        gmath::ray<gmath::world_space> const ray_to_light(hit.m_ray_hit.m_position + gmath::vector(hit.m_ray_hit.m_normal) * 0.0001f, world_light_dir, std::numeric_limits<float>::epsilon(), max_t, 0);
        constexpr glm::vec3 ambient_light(0.05f, 0.05f, 0.05f);
        gscene::blinn_phong_material const& material = gtl::cast<gscene::blinn_phong_material const&>(hit.m_object->get_material());
        glm::vec3 const ambient_color = ambient_light * material.m_diffuse_color;
        if (scene.raycast(ray_to_light))
        {
            return ambient_color;
        }

        gmath::direction<gmath::camera_space> const normal = camera.get_camera_transform() * hit.m_ray_hit.m_normal;
        gmath::direction<gmath::camera_space> const light_dir = camera.get_camera_transform() * world_light_dir;
        float const lambertian = std::max(light_dir.dot(normal), 0.f);
        float specular_coefficient = 0.f;
        if (lambertian > 0.f)
        {
            gmath::direction<gmath::camera_space> const view_dir = -gmath::vector(camera.get_camera_transform() * hit.m_ray_hit.m_position);
            gmath::direction<gmath::camera_space> const reflect_dir = -(light_dir).reflect(normal);
            float const specular_angle = std::max(view_dir.dot(reflect_dir), 0.f);
            specular_coefficient = std::pow(specular_angle, material.m_specular_exponent) * lambertian;
        }

        glm::vec3 const diffuse_color = material.m_diffuse_color * lambertian;
        glm::vec3 const specular_color = material.m_specular_color * specular_coefficient;

        return diffuse_color + specular_color + ambient_color;
    }
}