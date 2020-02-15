#pragma once
#include <optional>

#include "gmath/ray.h"

#include "gscene/ray_hit.h"

namespace gscene
{
    struct scene;
    struct accelerator
    {
        virtual ~accelerator() = 0 {};
        virtual std::optional<gscene::ray_hit> raycast(gmath::ray<gmath::world_space> const& ray) const noexcept = 0;
        virtual void build(gscene::scene const& scene) = 0;
    };
}