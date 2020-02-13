#pragma once
#include <vector>

#include "gmath/ray.h"

#include "gscene/common.h"

#include "grender/camera.h"

namespace grender
{
    struct camera;
    struct lense
    {
        virtual ~lense() = 0 {}
        virtual std::vector<gmath::ray<gmath::world_space>> generate_rays(camera const& camera, camera::generate_rays_params const& params) const = 0;
    };
}