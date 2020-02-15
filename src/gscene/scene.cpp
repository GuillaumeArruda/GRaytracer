#include "stdafx.h"

#include <glm/gtc/matrix_transform.hpp>

#include "gscene/scene.h"
#include "gscene/light.h"


namespace gscene
{
    scene::~scene() = default;

    std::optional<ray_hit> scene::raycast(gmath::ray<gmath::world_space> const& ray) const noexcept
    {
        return m_accelerator->raycast(ray);
    }

}
