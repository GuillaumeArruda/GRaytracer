
#include <fstream>

#include "CImg\CImg.h"

#include "gscene\scene.h"
#include "gscene\camera.h"
#include "gscene\lenses\pinhole_lense.h"
#include "gscene\light.h"
#include "gscene\material.h"

#include "gthread\job_manager.h"

#include "glm\gtc\matrix_transform.hpp"

int main(int argc, const char** argv)
{
    std::size_t constexpr image_width_resolution = 3440;
    std::size_t constexpr image_height_resolution = 1440;
    std::size_t constexpr image_resolution = image_width_resolution * image_height_resolution;
    std::size_t constexpr pixel_per_batch = 1000;
    std::size_t constexpr nb_aliasing_iteration = 10;
    std::size_t constexpr number_of_ray_per_pixel = nb_aliasing_iteration * nb_aliasing_iteration;
    bool constexpr display_image = true;

    std::size_t const number_of_thread = std::max(static_cast<long long>(std::thread::hardware_concurrency() - 2 * display_image), 1ll);
    std::size_t const number_of_batch = image_resolution / pixel_per_batch + (image_resolution % pixel_per_batch > 0);
    std::size_t const batch_size = pixel_per_batch;
    
    
    cimg_library::CImg<float> image(image_width_resolution, image_height_resolution, 1, 3, 0.f);

    gscene::scene scene("");
    auto lense = std::make_unique<gscene::pinhole_lense>(90.0_d, 1.f, std::numeric_limits<float>::max());
    gscene::camera camera(gscene::world_transform(glm::translate(glm::mat4(1.f), glm::vec3(0.f,0.f,-10.f))), std::move(lense));
    
    {
        gthread::job_manager job_manager(static_cast<unsigned int>(number_of_thread));
        for (int batch_id = 0; batch_id < number_of_batch; ++batch_id)
        {
            job_manager.submit(
                [&, batch_id, batch_size]()
            {

                float constexpr raycolor_contribution = 1.f / number_of_ray_per_pixel;

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
                    std::size_t const red_index = (x_index + y_index * params.x_resolution);
                    std::size_t const green_index = red_index + image_resolution;
                    std::size_t const blue_index = green_index + image_resolution;
                    for (std::size_t j = 0; j < number_of_ray_per_pixel; ++j)
                    {
                        if (std::optional<gscene::ray_hit> hit = scene.raycast(rays[base_index + j]))
                        {
                            for (auto const& light : scene.get_lights())
                            {
                                gmath::ray<gmath::world_space> const ray_to_light(hit->m_position + gmath::vector<gmath::world_space>(hit->m_normal) * 0.0001f, (light->get_transform().get_translation() - hit->m_position).normalize());
                                float const color_contribution = scene.raycast(ray_to_light) ? raycolor_contribution * 0.1f : raycolor_contribution;
                                image[red_index] = color_contribution * hit->m_object->get_material().m_color[0];
                                image[green_index] = color_contribution * hit->m_object->get_material().m_color[1];
                                image[blue_index] = color_contribution * hit->m_object->get_material().m_color[2];
                            }
                        }
                    }
                }
            }
            );
        }
        if (display_image)
        {
            cimg_library::CImgDisplay display(image);
            while (!display.is_closed())
            {
                display.display(image);
                display.wait(10);
            }
        }
    }

    image.save("output.bmp");
    return 0;
}