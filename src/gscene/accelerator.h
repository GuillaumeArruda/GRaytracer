#pragma once
#include <optional>

#include "gmath/ray.h"

#include "gscene/ray_hit.h"

#include "gserializer/type_factory.h"

namespace gserializer
{
    struct serializer;
}

namespace gscene
{
    struct scene;
    struct accelerator
    {
        GSERIALIZER_DECLARE_FACTORY_BASE(accelerator);
        virtual ~accelerator() = 0 {};
        virtual std::optional<gscene::ray_hit> raycast(gmath::ray<gmath::world_space> const& ray) const noexcept = 0;
        virtual void build(gscene::scene const& scene) = 0;
        virtual void process(gserializer::serializer& serializer) = 0;
    };
}