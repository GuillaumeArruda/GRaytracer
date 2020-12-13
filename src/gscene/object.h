#pragma once
#include <memory>
#include <optional>

#include "gmath/ray.h"
#include "gmath/axis_aligned_box.h"

#include "gscene/common.h"
#include "gscene/ray_hit.h"

namespace gserializer
{
    struct serializer;
}

namespace gscene
{
    struct shape;
    struct material;
    struct resource_library;
    struct object
    {
        object(world_transform const& transform, std::unique_ptr<shape> shape, material const* material);
        object() noexcept = default;
        object(object const&) = delete;
        object(object&&) noexcept;
        ~object();
        
        object& operator=(object const&) = delete;
        object& operator=(object&&) noexcept;

        world_transform const& get_transform() const noexcept { return m_transform; }
        model_transform const& get_inverse_transform() const noexcept { return m_inverseTransform; }
        gmath::axis_aligned_box<gmath::world_space> get_world_bounds() const noexcept;

        std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray) const noexcept;
    
        material const& get_material() const noexcept { return *m_material; }

        void resolve_resources(resource_library const& res_lib);
        void subdivide(std::vector<object>& new_object) const;

        void process(gserializer::serializer& serializer);

    private:
        std::unique_ptr<shape> m_shape;
        world_transform m_transform;
        model_transform m_inverseTransform;
        material const* m_material;
        std::string m_material_name;
    };
}