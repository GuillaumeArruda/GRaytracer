#include "grender/camera.h"
#include "grender/lense.h"
namespace grender
{
    camera::camera(gscene::world_transform const& transform, std::unique_ptr<lense> lense, std::size_t width, std::size_t height) noexcept
        : m_camera_transform(gscene::camera_transform(static_cast<glm::mat4>(transform.inverse())))
        , m_world_transform(transform)
        , m_lense(std::move(lense))
        , m_sensor(width, height)
    {
    }
    std::vector<gmath::ray<gmath::world_space>> camera::generate_rays(generate_rays_params const& params) const
    {
        return m_lense->generate_rays(*this, params);
    }
}

