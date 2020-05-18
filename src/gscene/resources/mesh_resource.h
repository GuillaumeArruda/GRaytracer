#pragma once

#include <string>
#include <vector>
#include <array>

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>

#include "gtl/span.h"
#include "gmath/position.h"
#include "gmath/direction.h"
#include "gscene/resource.h"

namespace gscene
{
    struct mesh_resource : resource
    {
        struct face
        {
            face(std::uint32_t v0, std::uint32_t v1, std::uint32_t v2, std::vector<gmath::position<gmath::model_space>> const& vertices);
            ~face() = default;
            face(face const&) = default;
            face(face&&) = default;
            
            face& operator=(face const&) = default;
            face& operator=(face&&) = default;

            static gmath::vector<gmath::model_space> compute_normal(gmath::position<gmath::model_space> const& v0, gmath::position<gmath::model_space> const& v1, gmath::position<gmath::model_space> const& v2);
            std::array<std::uint32_t, 3> m_indices;
            gmath::vector<gmath::model_space> m_normal;
            float m_area2;
            float m_d;
        };

        struct submesh
        {
            std::vector<gmath::position<gmath::model_space>> m_vertices_position;
            std::vector<gmath::direction<gmath::model_space>> m_vertices_normal;
            std::vector<gmath::vector<gmath::model_space>> m_edges;
            std::vector<face> m_faces_index;
        };

        mesh_resource() = default;
        ~mesh_resource() = default;
        mesh_resource(mesh_resource&&) noexcept = default;
        mesh_resource& operator=(mesh_resource&&) noexcept = default;
        
        mesh_resource(mesh_resource const&) = delete;
        mesh_resource& operator=(mesh_resource const&) = delete;

        template<typename Archive>
        void serialize(Archive& ar)
        {
            ar(CEREAL_NVP(m_filepath));
        }

        void load() final;

        gtl::span<gmath::position<gmath::model_space> const> get_vertices_position(std::size_t submesh_id) const { return m_submeshes[submesh_id].m_vertices_position; }
        gtl::span<gmath::direction<gmath::model_space> const> get_vertices_normal(std::size_t submesh_id) const { return m_submeshes[submesh_id].m_vertices_normal; }
        gtl::span<gmath::vector<gmath::model_space> const> get_edges(std::size_t submesh_id) const { return m_submeshes[submesh_id].m_edges; }
        gtl::span<face const> get_faces_index(std::size_t submesh_id) const { return m_submeshes[submesh_id].m_faces_index; }
        std::size_t number_of_submeshes() const { return m_submeshes.size(); }
    private:
        std::vector<submesh> m_submeshes;
        std::string m_filepath;
    };
}

CEREAL_REGISTER_TYPE(gscene::mesh_resource);
CEREAL_REGISTER_POLYMORPHIC_RELATION(gscene::resource, gscene::mesh_resource);