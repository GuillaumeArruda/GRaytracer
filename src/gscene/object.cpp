#include "stdafx.h"

#include "gscene/object.h"
#include "gscene/shape.h"
#include "gscene/resources/material.h"
#include "gscene/resource_library.h"

namespace gscene
{
    object::object(world_transform const& transform, std::unique_ptr<shape> shape, material const* material)
        : m_transform(transform), m_inverseTransform(m_transform.inverse()), m_shape(std::move(shape)), m_material(material) {}
    object::object(object&&) noexcept = default;
    object::~object() = default;

    object& object::operator=(object&&) = default;
    
    gmath::axis_aligned_box<gmath::world_space> object::get_world_bounds() const noexcept
    {
        return m_shape->world_bounds(m_transform);
    }

    std::optional<ray_hit> object::raycast(gmath::ray<gmath::world_space> const& ray) const noexcept
    {
        return m_shape->raycast(ray, *this);
    }
    void object::resolve_resources(resource_library const& res_lib)
    {
        m_material = res_lib.get_resource<material>(m_material_name);
        m_shape->resolve_resources(res_lib);
    }

    void object::subdivide(std::vector<object>& new_object) const
    {
        m_shape->subdivide(*this, new_object);
    }
}

