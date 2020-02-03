#include "gscene/camera.h"
#include "gscene/lense.h"
namespace gscene
{
    camera::camera(world_transform const& transform, std::unique_ptr<lense> lense) noexcept
        : m_camera_transform(camera_transform(static_cast<glm::mat4>(transform.inverse())))
        , m_world_transform(transform)
        , m_lense(std::move(lense))
    {
    }
    std::vector<gmath::ray<gmath::world_space>> camera::generate_rays(generate_rays_params const& params) const
    {
        return m_lense->generate_rays(m_world_transform, params);
    }
}

