#pragma once
#include "gmath/transform.h"
namespace gscene
{
    using world_transform = gmath::transform<gmath::world_space, gmath::model_space>;
    using model_transform = gmath::transform<gmath::model_space, gmath::world_space>;
    using camera_transform = gmath::transform<gmath::camera_space, gmath::world_space>;
}
