#include "stdafx.h"

#include "gscene/object.h"
#include "gscene/shape.h"
#include "gscene/material.h"

namespace gscene
{
    object::object(world_transform const& transform, std::unique_ptr<shape> shape, std::unique_ptr<material> material)
        : m_transform(transform), m_inverseTransform(m_transform.inverse()), m_shape(std::move(shape)), m_material(std::move(material)) {}
    object::~object() = default;
    
    gmath::axis_aligned_box<gmath::world_space> object::get_world_bounds() const noexcept
    {
        return m_shape->world_bounds(m_transform);
    }

    std::optional<ray_hit> object::raycast(gmath::ray<gmath::world_space> const& ray) const noexcept
    {
        return m_shape->raycast(ray, *this);
    }
}

