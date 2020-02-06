#pragma once
#include <memory>
#include <optional>

#include "gmath\ray.h"

#include "gscene\common.h"
#include "gscene\ray_hit.h"

namespace gscene
{
    struct shape;
    struct material;
    struct object
    {
        object(world_transform const& transform, std::unique_ptr<shape> shape, std::unique_ptr<material> material);
        object(object const&) = delete;
        object(object&&) noexcept = default;
        ~object();
        
        object& operator=(object const&) = delete;
        object& operator=(object&&) = default;

        world_transform const& get_transform() const noexcept { return m_transform; }
        model_transform const& get_inverse_transform() const noexcept { return m_inverseTransform; }

        std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray) const noexcept;
    
        material const& get_material() const noexcept { return *m_material; }
    private:
        world_transform m_transform;
        model_transform m_inverseTransform;
        std::unique_ptr<shape> m_shape;
        std::unique_ptr<material> m_material;
    };
}