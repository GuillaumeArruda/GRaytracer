
#include <fstream>

#include "CImg\CImg.h"

#include "gscene\scene.h"
#include "gthread\job_manager.h"



int main(int argc, const char** argv)
{
    std::size_t constexpr image_width_resolution = 3440;
    std::size_t constexpr image_height_resolution = 1440;
    std::size_t constexpr image_resolution = image_width_resolution * image_height_resolution;
    std::size_t const number_of_thread = std::thread::hardware_concurrency();
    std::size_t const number_of_batch = number_of_thread;
    std::size_t const batch_size = image_resolution / number_of_batch;
    
    
    gthread::job_manager job_manager(static_cast<unsigned int>(number_of_thread));
    cimg_library::CImg<unsigned char> image(image_width_resolution, image_height_resolution,1 ,1, 0);

    std::vector<std::future<void>> m_jobs;
    m_jobs.reserve(number_of_batch);
    gscene::scene scene("");
    const gmath::vector<gmath::world_space> ray_direction(0.f, 0.f, 1.f); 

    for (int batch_id = 0; batch_id < number_of_batch; ++batch_id)
    {
        m_jobs.emplace_back(
            job_manager.submit_with_future(
                [&, batch_id, batch_size]()
                {
                    std::size_t constexpr nb_aliasing_iteration = 15;
                    float constexpr aliasing_displacement = 1.f / nb_aliasing_iteration;
                    unsigned char constexpr raycolor_contribution = static_cast<unsigned char>(std::numeric_limits<unsigned char>::max() / (nb_aliasing_iteration * nb_aliasing_iteration));

                    std::size_t const start_index = batch_id * batch_size;
                    std::size_t const start_x = start_index / image_width_resolution;
                    std::size_t const start_y = start_index % image_width_resolution;
                    for (std::size_t i = 0; i < batch_size; ++i)
                    {
                        std::size_t const x_pos = start_x + i / image_width_resolution;
                        std::size_t const y_pos = start_y + i % image_width_resolution;
                        for (std::size_t j = 0; j < nb_aliasing_iteration; ++j)
                        {
                            for (std::size_t k = 0; k < nb_aliasing_iteration; ++k)
                            {
                                gmath::ray<gmath::world_space> const ray(
                                    gmath::position<gmath::world_space>((static_cast<float>(x_pos) + j * aliasing_displacement) / 20, (static_cast<float>(y_pos) + k * aliasing_displacement) / 20, 0.f),
                                    ray_direction);
                                image[x_pos * image_width_resolution + y_pos] += static_cast<bool>(scene.raycast(ray)) * raycolor_contribution;
                            }
                        }
                    }
                }
            )
        );
    }

    for (auto const& job : m_jobs)
    {
        job.wait();
    }
    image.save("output.bmp");
    return 0;
}