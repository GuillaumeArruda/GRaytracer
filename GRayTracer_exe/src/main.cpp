
#include <iostream>

#include "gscene\scene.hpp"

#include "gthread\job_manager.hpp"

int main(int argc, const char** argv)
{
    
    constexpr int image_size = 80;
    std::unique_ptr<bool[]> image(new bool[image_size * image_size]);
    const int number_of_thread = 8;
    gthread::job_manager job_manager(number_of_thread);
    const int number_of_batch = number_of_thread;
    const int batch_size = image_size * image_size / number_of_batch;
    std::vector<std::future<void>> m_jobs;
    gscene::scene scene("");
    const gmath::vector<gmath::world_space> ray_direction(0.f, 0.f, 1.f);

    for (int batch_id = 0; batch_id < number_of_batch; ++batch_id)
    {
        m_jobs.emplace_back(
            job_manager.submit_with_future(
                [&, batch_id, batch_size]()
                {
                    const int start_index = batch_id * batch_size;
                    const int start_x = start_index / image_size;
                    const int start_y = start_index % image_size;
                    for (int i = 0; i < batch_size; ++i)
                    {
                        int const x_pos = start_x + i / image_size;
                        int const y_pos = start_y + i % image_size;
                        gmath::ray<gmath::world_space> const ray(gmath::position<gmath::world_space>(static_cast<float>(x_pos), static_cast<float>(y_pos), 0.f), ray_direction);
                        image[x_pos * image_size + y_pos] = static_cast<bool>(scene.raycast(ray));
                    }
                }
            )
        );
    }

    for (auto const& job : m_jobs)
    {
        job.wait();
    }

    for (int i = 0; i < image_size; ++i)
    {
        for (int j = 0; j < image_size; ++j)
        {
            std::cout << image[i * image_size + j];
        }
        std::cout << "\n";
    }

    return 0;
}