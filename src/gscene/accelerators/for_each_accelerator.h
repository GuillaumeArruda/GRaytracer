#pragma once

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>

#include "gtl/span.h"

#include "gscene/accelerator.h"
#include "gscene/object.h"

namespace gscene
{
    struct for_each_accelerator final : accelerator
    {
        std::optional<gscene::ray_hit> raycast(gmath::ray<gmath::world_space> const&) const noexcept final;
        void build(scene const& scene) final;
        
        template<class Archive>
        void serialize(Archive& ar) {}
    private:
        gtl::span<object const> m_objects;
    };
}

CEREAL_REGISTER_TYPE(gscene::for_each_accelerator);
CEREAL_REGISTER_POLYMORPHIC_RELATION(gscene::accelerator, gscene::for_each_accelerator);