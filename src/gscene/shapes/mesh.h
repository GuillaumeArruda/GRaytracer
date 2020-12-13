#pragma once

#include "gscene/shape.h"

namespace gscene
{
    struct mesh_resource;
    struct mesh final : shape
    {
        mesh(std::string mesh_name, mesh_resource const* data, std::size_t submesh_id) noexcept : m_mesh_name(std::move(mesh_name)), m_data(data), m_submesh_id(submesh_id) {}
        mesh() noexcept = default;
        mesh(mesh&&) noexcept = default;
        mesh(mesh const&) noexcept = default;
        ~mesh() noexcept = default;

        mesh& operator=(mesh&&) noexcept = default;
        mesh& operator=(mesh const&) noexcept = default;

        std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept final;
        gmath::axis_aligned_box<gmath::world_space> world_bounds(world_transform const& transform) const noexcept final;
        void resolve_resources(resource_library const& res_lib) override final;

        void subdivide(object const& obj, std::vector<object>& new_object) const override final;
        void process(gserializer::serializer& serializer) override;

    private:
        std::string m_mesh_name;
        mesh_resource const* m_data = nullptr;
        std::size_t m_submesh_id = 0;

        GSERIALIZER_DECLARE_SUBCLASS_FACTORY_REGISTRATION();
    };
}