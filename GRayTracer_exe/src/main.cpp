#include <fstream>

#include "gthread/job_manager.h"

#include "gscene/scene.h"
#include "gscene/light.h"
#include "gscene/shape.h"

#include "gscene/serialization_headers.h"

#include "grender/lenses/pinhole_lense.h"
#include "grender/camera.h"
#include "grender/integrators/blinn_phong_integrator.h"

#include "glm/gtc/matrix_transform.hpp"

#include <fstream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

int main(int argc, const char** argv)
{
    std::size_t constexpr image_width_resolution = 1280;
    std::size_t constexpr image_height_resolution = 720;
    std::size_t constexpr image_resolution = image_width_resolution * image_height_resolution;
    std::size_t constexpr pixel_per_batch = 128;

    bool constexpr display_image = false;

    std::size_t const number_of_thread = std::max(static_cast<long long>(std::thread::hardware_concurrency()), 1ll);
    std::size_t const number_of_batch = image_resolution / pixel_per_batch + (image_resolution % pixel_per_batch > 0);
    std::size_t const batch_size = pixel_per_batch;
    
    gscene::scene scene;
    {
        std::ifstream is("data/scene.json");
        cereal::JSONInputArchive iarchive(is);
        iarchive(scene);
    }


    auto lense = std::make_unique<grender::pinhole_lense>(70.0_d, 1.f, std::numeric_limits<float>::max());
    grender::camera const camera(gscene::world_transform(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 2.5f, -4.f))), std::move(lense), image_width_resolution, image_height_resolution);
    grender::blinn_phong_integrator integrator;

    gthread::job_manager job_manager(static_cast<unsigned int>(number_of_thread));
    scene.prepare_scene(job_manager);
    integrator.verify_scene(scene);
    for (std::size_t batch_id = 0; batch_id < number_of_batch; ++batch_id)
    {
        job_manager.submit(
            [&, batch_id, batch_size]
        {
            std::size_t const start_pixel = batch_id * batch_size;
            std::size_t const number_of_pixel = std::min(batch_size, image_resolution - start_pixel);
            integrator.render(scene, camera, glm::uvec2(start_pixel, start_pixel + number_of_pixel));
        }
        );
    }

    if (display_image)
    {
        cimg_library::CImgDisplay display(camera.get_sensor().get_image());
        display.set_normalization(2);
        while (!display.is_closed())
        {
            display.display(camera.get_sensor().get_image());
            display.wait(10);
        }
    }

    return 0;
}