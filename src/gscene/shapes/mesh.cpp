#include "stdafx.h"

#include "gmath/vector.h"

#include "gscene/shapes/mesh.h"
#include "gscene/object.h"
#include "gscene/resources/mesh_resource.h"
#include "gscene/resource_library.h"

namespace gscene
{
    std::optional<ray_hit> mesh::raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept
    {
        gmath::ray<gmath::model_space> const local_ray = obj.get_inverse_transform() * ray;

        gtl::span<gmath::position<gmath::model_space> const> vertices = m_data->get_vertices_position(m_submesh_id);
        gtl::span<gmath::direction<gmath::model_space> const> normals = m_data->get_vertices_normal(m_submesh_id);
        gtl::span<gmath::vector<gmath::model_space> const> edges = m_data->get_edges(m_submesh_id);
        gtl::span<mesh_resource::face const> faces = m_data->get_faces_index(m_submesh_id);

        std::optional<ray_hit> hit;
        for (std::size_t i = 0; i < faces.size(); ++i)
        {
            mesh_resource::face const& face = faces[i];
            gmath::vector<gmath::model_space> const& normal = face.m_normal;
            float const denom = normal.dot(local_ray.dir());
            if (std::abs(denom) < std::numeric_limits<float>::epsilon())
                continue;

            float const t = -(normal.dot(gmath::vector(local_ray.start())) + face.m_d) / denom;

            if ((hit && hit->m_ray_hit.m_t < t) || !ray.is_valid_t(t))
                continue;


            gmath::position const hit_pos = local_ray(t);

            gmath::position const v0(vertices[face.m_indices[0]]);

            std::size_t const edge_base_index = i * 3;
            gmath::vector<gmath::model_space> const& edge0 = edges[edge_base_index];
            gmath::vector const vp0 = hit_pos - v0;
            if (edge0.cross(vp0).dot(normal) < 0)
                continue;

            gmath::position const v1(vertices[face.m_indices[1]]);
            gmath::vector<gmath::model_space> const& edge1 = edges[edge_base_index + 1];
            gmath::vector const vp1 = hit_pos - v1;
            float const unnormalized_u = edge1.cross(vp1).dot(normal);
            if (unnormalized_u < 0)
                continue;

            gmath::position const v2(vertices[face.m_indices[2]]);
            gmath::vector<gmath::model_space> const& edge2 = edges[edge_base_index + 2];
            gmath::vector const vp2 = hit_pos - v2;
            float const unnormalized_v = edge2.cross(vp2).dot(normal);
            if (unnormalized_v < 0)
                continue;

            float const u = unnormalized_u / face.m_area2;
            float const v = unnormalized_v / face.m_area2;
            gmath::direction<gmath::model_space> const normal_at_hit(u * gmath::vector(normals[face.m_indices[0]]) + v * gmath::vector(normals[face.m_indices[1]]) + (1.f - u - v) * gmath::vector(normals[face.m_indices[2]]));

            hit = ray_hit{ {ray(t), obj.get_transform() * normal_at_hit, &ray, t}, &obj };
        }

        return hit;
    }

    gmath::axis_aligned_box<gmath::world_space> mesh::world_bounds(world_transform const& transform) const noexcept
    {
        gtl::span<gmath::position<gmath::model_space> const> vertices_position = m_data->get_vertices_position(m_submesh_id);
        gmath::position<gmath::world_space> min_pos(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
        gmath::position<gmath::world_space> max_pos(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());

        for (auto const& vertice : vertices_position)
        {
            using vec_type = gmath::position<gmath::world_space>::glm_vector_type;
            gmath::position<gmath::world_space> const world_space_pos = transform * vertice;
            min_pos = glm::min(static_cast<vec_type>(min_pos), static_cast<vec_type>(world_space_pos));
            max_pos = glm::max(static_cast<vec_type>(max_pos), static_cast<vec_type>(world_space_pos));
        }

        return gmath::axis_aligned_box<gmath::world_space>(min_pos, max_pos);
    }

    void mesh::resolve_resources(resource_library const& res_lib)
    {
        m_data = res_lib.get_resource<mesh_resource>(m_mesh_name);
    }

    void mesh::subdivide(object const& obj, std::vector<object>& new_object) const
    {
        for (std::size_t i = 1; i < m_data->number_of_submeshes(); ++i)
        {
            std::unique_ptr new_mesh = std::make_unique<mesh>(m_mesh_name, m_data, i);
            new_object.emplace_back(obj.get_transform(), std::move(new_mesh), &obj.get_material());
        }
    }
}
