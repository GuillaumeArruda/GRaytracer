
#include <fstream>

#include "CImg\CImg.h"

#include "gscene\scene.h"
#include "gscene\camera.h"
#include "gscene\lenses\pinhole_lense.h"
#include "gscene\light.h"

#include "gthread\job_manager.h"

#include "glm\gtc\matrix_transform.hpp"

int main(int argc, const char** argv)
{
    std::size_t constexpr image_width_resolution = 3440;
    std::size_t constexpr image_height_resolution = 1440;
    std::size_t constexpr image_resolution = image_width_resolution * image_height_resolution;
    std::size_t constexpr pixel_per_batch = 7000;
    std::size_t constexpr nb_aliasing_iteration = 8;
    std::size_t constexpr number_of_ray_per_pixel = nb_aliasing_iteration * nb_aliasing_iteration;

    std::size_t const number_of_thread = std::thread::hardware_concurrency();
    std::size_t const number_of_batch = image_resolution / pixel_per_batch + (image_resolution % pixel_per_batch > 0);
    std::size_t const batch_size = pixel_per_batch;
    
    
    cimg_library::CImg<unsigned char> image(image_width_resolution, image_height_resolution,1 ,1, 0);

    gscene::scene scene("");
    auto lense = std::make_unique<gscene::pinhole_lense>(70.0_d, 1.f, std::numeric_limits<float>::max());
    gscene::camera camera(gscene::world_transform(glm::translate(glm::mat4(1.f), glm::vec3(0.f,0.f,-10.f))), std::move(lense));
    
    {
        gthread::job_manager job_manager(static_cast<unsigned int>(number_of_thread));
        for (int batch_id = 0; batch_id < number_of_batch; ++batch_id)
        {
            job_manager.submit(
                [&, batch_id, batch_size]()
            {

                unsigned char constexpr raycolor_contribution = static_cast<unsigned char>(std::numeric_limits<unsigned char>::max() / number_of_ray_per_pixel);

                gscene::camera::generate_rays_params params;
                params.start_pixel = batch_id * batch_size;
                params.number_of_pixel = batch_size;
                if (params.start_pixel + batch_size > image_resolution)
                {
                    params.number_of_pixel = image_resolution - params.start_pixel;
                }
                params.x_resolution = image_width_resolution;
                params.y_resolution = image_height_resolution;
                params.axis_aliasing_sample = nb_aliasing_iteration;

                std::vector<gmath::ray<gmath::world_space>> const rays = camera.generate_rays(params);
                for (std::size_t i = 0; i < params.number_of_pixel; ++i)
                {
                    std::size_t const base_index = i * number_of_ray_per_pixel;
                    std::size_t const x_index = (params.start_pixel + i) % params.x_resolution;
                    std::size_t const y_index = (params.start_pixel + i) / params.x_resolution;
                    std::size_t const image_index = x_index + y_index * params.x_resolution;
                    for (std::size_t j = 0; j < number_of_ray_per_pixel; ++j)
                    {
                        if (std::optional<gscene::ray_hit> hit = scene.raycast(rays[base_index + j]))
                        {
                            for (auto const& light : scene.get_lights())
                            {
                                gmath::ray<gmath::world_space> const ray_to_light(hit->m_position + gmath::vector<gmath::world_space>(hit->m_normal) * 0.0001f, (light->get_transform().get_translation() - hit->m_position).normalize());
                                if (scene.raycast(ray_to_light)) // We hit something there is no direct path to the light.
                                {
                                    image[image_index] += static_cast<unsigned char>(raycolor_contribution * 0.1f);
                                }
                                else
                                {
                                    image[image_index] += raycolor_contribution;
                                }
                            }
                        }
                    }

                }
            }
            );
        }
    }
    
    image.save("output.bmp");
    return 0;
}