#pragma once

#include "gtl/span.h"
#include "gscene/accelerator.h"
#include "gscene/object.h"

namespace gscene
{
    struct for_each_accelerator final : accelerator
    {
        std::optional<gscene::ray_hit> raycast(gmath::ray<gmath::world_space> const&) const noexcept final;
        void build(scene const& scene) final;

        void process(gserializer::serializer&) override {}
    private:
        gtl::span<object const> m_objects;

        GSERIALIZER_DECLARE_SUBCLASS_FACTORY_REGISTRATION();
    };
}