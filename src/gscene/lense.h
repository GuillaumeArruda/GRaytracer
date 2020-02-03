#pragma once
#include <vector>

#include "gscene\common.h"
#include "gscene\camera.h"

#include "gmath\ray.h"

namespace gscene
{
    struct lense
    {
        virtual ~lense() {}
        virtual std::vector<gmath::ray<gmath::world_space>> generate_rays(world_transform const& transform, camera::generate_rays_params const& params) const = 0;
    };
}