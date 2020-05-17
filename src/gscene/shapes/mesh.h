#pragma once

#include <memory>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>

#include "gscene/shape.h"

namespace gscene
{
    struct mesh_resource;
    struct mesh final : shape
    {
        mesh(std::string mesh_name) noexcept : m_mesh_name(std::move(mesh_name)) {}
        mesh() noexcept = default;
        mesh(mesh&&) noexcept = default;
        mesh(mesh const&) noexcept = default;
        ~mesh() noexcept = default;

        mesh& operator=(mesh&&) noexcept = default;
        mesh& operator=(mesh const&) noexcept = default;

        std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept final;
        gmath::axis_aligned_box<gmath::world_space> world_bounds(world_transform const& transform) const noexcept final;
        void resolve_resources(resource_library const& res_lib) final;

        template<typename Archive>
        void serialize(Archive& ar)
        {
            ar(CEREAL_NVP(m_mesh_name));
        }

    private:
        std::string m_mesh_name;
        mesh_resource const* m_data = nullptr;
    };
}

CEREAL_REGISTER_TYPE(gscene::mesh);
CEREAL_REGISTER_POLYMORPHIC_RELATION(gscene::shape, gscene::mesh);