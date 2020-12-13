#pragma once

#include <optional>

#include "gmath/axis_aligned_box.h"
#include "gmath/transform.h"
#include "gmath/ray.h"

#include "gscene/ray_hit.h"
#include "gscene/common.h"

#include "gserializer/type_factory.h"
#include "gserializer/serializer.h"

namespace gscene
{
    struct object;
    struct resource_library;
    struct shape
    {
        GSERIALIZER_DECLARE_FACTORY_BASE(shape);
        virtual ~shape() = 0 {};

        virtual std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept = 0;

        virtual gmath::axis_aligned_box<gmath::world_space> world_bounds(world_transform const& transform) const noexcept = 0;

        virtual void resolve_resources(resource_library const&) {}

        virtual void subdivide(object const&, std::vector<object>&) const {}

        virtual void process(gserializer::serializer&) = 0;

    protected:
        shape() noexcept = default;
        shape(shape const&) noexcept = default;
        shape(shape&&) noexcept = default;
        shape& operator=(shape const&) noexcept = default;
        shape& operator=(shape&&) noexcept = default;

    };
}