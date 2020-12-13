#include "stdafx.h"

#include <array>
#include <algorithm>

#include "gtl/cast.h"

#include "gscene/accelerators/kd_accelerator.h"
#include "gscene/scene.h"

namespace gscene
{
    kd_accelerator::kd_accelerator() noexcept
        : m_bound(gmath::position<gmath::world_space>(), gmath::position<gmath::world_space>())
    {
    }

    std::optional<gscene::ray_hit> kd_accelerator::raycast(gmath::ray<gmath::world_space> const& ray) const noexcept
    {
        if (auto bounds = m_bound.intersect(ray))
        {
            gmath::vector<gmath::world_space> const inverse_dir(1.f / static_cast<glm::vec3>(ray.dir()));

            struct node_to_do
            {
                node const* m_node;
                float m_t_min;
                float m_t_max;
            };

            std::optional<gscene::ray_hit> best_hit;
            std::size_t constexpr max_to_do = 64;
            std::array<node_to_do, max_to_do> to_dos;
            std::size_t current_to_do_index = 0;
            node_to_do current_to_do{ &m_nodes[0], bounds->t0, bounds->t1 };
            while (current_to_do.m_node != nullptr)
            {
                if (ray.maxT() < current_to_do.m_t_min) break;

                if (current_to_do.m_node->is_leaf())
                {
                    node::leaf_data const& leaf_data = current_to_do.m_node->get_leaf_data();
                    for (std::uint32_t i = 0; i < leaf_data.m_number_of_objects; ++i)
                    {
                        std::optional<ray_hit> hit = leaf_data.m_objects[i]->raycast(ray);
                        if (hit)
                            if (!best_hit)
                                best_hit = std::move(hit);
                            else
                                if (*hit < *best_hit)
                                    best_hit = std::move(hit);
                    }

                    if (current_to_do_index > 0)
                    {
                        current_to_do = to_dos[--current_to_do_index];
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    node::intermediate_data const& data = current_to_do.m_node->get_intermediate_data();
                    std::size_t const axis = static_cast<std::size_t>(data.m_split_axis);
                    float const t_plane = (data.m_split_position - ray.start()[axis]) * inverse_dir[axis];

                    bool const below_first = (ray.start()[axis] < data.m_split_position) || (ray.start()[axis] == data.m_split_position && ray.dir()[axis] <= 0.f);
                    node const* first_child;
                    node const* second_child;
                    if (below_first)
                    {
                        first_child = current_to_do.m_node + 1;
                        second_child = &m_nodes[data.m_right_child_index];
                    }
                    else
                    {
                        first_child = &m_nodes[data.m_right_child_index];
                        second_child = current_to_do.m_node + 1;
                    }
                    
                    if (t_plane > current_to_do.m_t_max || t_plane <= 0.f)
                        current_to_do.m_node = first_child;
                    else if (t_plane < current_to_do.m_t_min)
                        current_to_do.m_node = second_child;
                    else
                    {
                        to_dos[current_to_do_index++] = node_to_do{second_child, t_plane, current_to_do.m_t_max};
                        current_to_do.m_node = first_child;
                        current_to_do.m_t_max = t_plane;
                    }
                }
            }
            return best_hit;
        }
        return {};
    }

    void kd_accelerator::build(gscene::scene const& scene)
    {
        gtl::span<object const> objects = scene.get_objects();
        std::vector<gmath::axis_aligned_box<gmath::world_space>> bounds;
        bounds.reserve(objects.size());
        for (object const& obj : objects)
        {
            bounds.emplace_back(obj.get_world_bounds());
            m_bound = m_bound.merge(bounds.back());
        }

        std::array<std::vector<bound_edge>, 3> edges;
        for (int i = 0; i < 3; ++i)
        {
            edges[i].resize(objects.size() * 2);
        }

        std::vector<std::size_t> indexes;
        indexes.reserve(objects.size());
        for (std::size_t i = 0; i < objects.size(); ++i)
            indexes.emplace_back(i);

        recursive_build(objects, m_bound, bounds, indexes, m_max_depth, edges);
    }

    void kd_accelerator::recursive_build(gtl::span<object const> all_objects, gmath::axis_aligned_box<gmath::world_space> const& node_bound, std::vector<gmath::axis_aligned_box<gmath::world_space>> const& all_objects_bounds, gtl::span<std::size_t const> indexes, std::uint32_t depth, std::array<std::vector<bound_edge>, 3>& edges)
    {
        if (indexes.size() <= m_max_cost || depth == 0)
        {
            create_leaf(all_objects, indexes);
            return;
        }

        gmath::vector<gmath::world_space> const d = node_bound.max() - node_bound.min();

        std::optional<std::size_t> best_axis;
        std::optional<std::size_t> best_offset;
        float best_cost = std::numeric_limits<float>::infinity();
        float old_cost = m_isect_cost * indexes.size();
        float const total_surface_area = node_bound.surface_area();
        float const inv_total_surface_area = 1.f / total_surface_area;

        std::size_t const first_axis = node_bound.maximum_extent();
        for (std::size_t number_of_tries = 0; number_of_tries < 3; ++number_of_tries)
        {
            std::size_t const axis = (first_axis + number_of_tries) % 3;
            for (std::size_t j = 0; j < indexes.size(); ++j)
            {
                std::size_t const index = indexes[j];
                edges[axis][2 * j] = bound_edge{ static_cast<std::uint32_t>(index), all_objects_bounds[index].min()[axis], bound_edge::type::begin };
                edges[axis][2 * j + 1] = bound_edge{ static_cast<std::uint32_t>(index), all_objects_bounds[index].max()[axis], bound_edge::type::end };
            }
            std::sort(edges[axis].begin(), edges[axis].begin() + (indexes.size() * 2), [](bound_edge const& lhs, bound_edge const& rhs)
            {
                if (lhs.m_t == rhs.m_t)
                    return lhs.m_type < rhs.m_type;
                return lhs.m_t < rhs.m_t;
            }
            );

            std::size_t number_below = 0;
            std::size_t number_above = indexes.size();
            for (std::size_t i = 0; i < 2 * indexes.size(); ++i)
            {
                if (edges[axis][i].m_type == bound_edge::type::end) --number_above;

                float const edge_t = edges[axis][i].m_t;
                if (edge_t > node_bound.min()[axis] && edge_t < node_bound.max()[axis])
                {
                    std::size_t const second_axis = (axis + 1) % 3;
                    std::size_t const third_axis = (axis + 2) % 3;

                    float const below_surface_area = 2 * (d[second_axis] * d[third_axis] + (edge_t - node_bound.min()[axis]) * (d[second_axis] + d[third_axis]));
                    float const above_surface_area = 2 * (d[second_axis] * d[third_axis] + (node_bound.max()[axis] - edge_t) * (d[second_axis] + d[third_axis]));
                    float const probability_below = below_surface_area * inv_total_surface_area;
                    float const probability_above = above_surface_area * inv_total_surface_area;
                    float const empty_bonus = (number_below == 0 || number_above == 0) ? m_empty_bonus : 0.f;
                    float const cost = m_traversal_cost + m_isect_cost * (1.f - empty_bonus) * (probability_below * number_below + probability_above * number_above);
                    if (cost < best_cost)
                    {
                        best_cost = cost;
                        best_axis = axis;
                        best_offset = i;
                    }
                }
                if (edges[axis][i].m_type == bound_edge::type::begin) ++number_below;
            }

            if (best_axis)
                break;
        }

        if ((best_cost > 4 * old_cost && indexes.size()) || !best_axis)
        {
            create_leaf(all_objects, indexes);
            return;
        }

        std::vector<std::size_t> below_indexes;
        for (std::size_t i = 0; i < *best_offset; ++i)
            if (edges[*best_axis][i].m_type == bound_edge::type::begin)
                below_indexes.emplace_back(edges[*best_axis][i].m_object_index);
        
        std::vector<std::size_t> above_indexes;
        for (std::size_t i = *best_offset + 1; i < 2 * indexes.size(); ++i)
            if (edges[*best_axis][i].m_type == bound_edge::type::end)
                above_indexes.emplace_back(edges[*best_axis][i].m_object_index);

        float const t_split = edges[*best_axis][*best_offset].m_t;

        std::size_t current_node_index = m_nodes.size();
        m_nodes.emplace_back(static_cast<node::split_axis>(*best_axis), t_split);
        
        gmath::position<gmath::world_space> below_new_max = node_bound.max();
        below_new_max[*best_axis] = t_split;
        gmath::axis_aligned_box<gmath::world_space> const below_box(node_bound.min(), below_new_max);
        recursive_build(all_objects, below_box, all_objects_bounds, below_indexes, depth - 1, edges);

        m_nodes[current_node_index].get_intermediate_data().m_right_child_index = m_nodes.size();
        gmath::position<gmath::world_space> above_new_min = node_bound.min();
        above_new_min[*best_axis] = t_split;
        gmath::axis_aligned_box<gmath::world_space> const above_box(node_bound.min(), below_new_max);
        recursive_build(all_objects, above_box, all_objects_bounds, above_indexes, depth - 1, edges);
    }

    void kd_accelerator::create_leaf(gtl::span<object const> all_objects, gtl::span<std::size_t const> indexes)
    {
        std::unique_ptr<object const* []> obj(new object const* [indexes.size()]);
        for (std::size_t i = 0; i < indexes.size(); ++i)
            obj[i] = &all_objects[indexes[i]];
        m_nodes.emplace_back(std::move(obj), static_cast<std::uint32_t>(indexes.size()));
    }

    kd_accelerator::node::node(std::unique_ptr<object const* []> objects, std::uint32_t size)
    {
        new(static_cast<void*>(m_storage)) leaf_data{true, size, std::move(objects) };
    }

    kd_accelerator::node::node(split_axis axis, float split_position)
    {
        new(static_cast<void*>(m_storage)) intermediate_data{ false, axis, 0, split_position};
    }

    kd_accelerator::node::~node()
    {
        if (is_leaf())
            get_leaf_data().~leaf_data();
        else
            get_intermediate_data().~intermediate_data();
  
    }
    kd_accelerator::node::node(node&& move) noexcept
    {
        if (move.is_leaf())
            new(static_cast<void*>(m_storage)) leaf_data(std::move(move.get_leaf_data()));
        else
            new(static_cast<void*>(m_storage)) intermediate_data(std::move(move.get_intermediate_data()));
    }
    kd_accelerator::node& kd_accelerator::node::operator=(node&& move) noexcept
    {
        if (is_leaf())
            get_leaf_data().~leaf_data();
        else
            get_intermediate_data().~intermediate_data();

        if (move.is_leaf())
            new(static_cast<void*>(m_storage)) leaf_data(std::move(move.get_leaf_data()));
        else
            new(static_cast<void*>(m_storage)) intermediate_data(std::move(move.get_intermediate_data()));

        return *this;
    }
    bool kd_accelerator::node::is_leaf() const noexcept
    {
        return reinterpret_cast<node_data const*>(m_storage)->m_is_leaf;
    }
    kd_accelerator::node::leaf_data& kd_accelerator::node::get_leaf_data() noexcept
    {
#if GTL_CHECK_CAST()
        if (!is_leaf())
            throw std::exception();
#endif // GTL_CAST_CHECK
        return *reinterpret_cast<leaf_data*>(m_storage);
    }
    kd_accelerator::node::leaf_data const& kd_accelerator::node::get_leaf_data() const noexcept
    {
#if GTL_CHECK_CAST()
        if (!is_leaf())
            throw std::exception();
#endif // GTL_CAST_CHECK
        return *reinterpret_cast<leaf_data const*>(m_storage);
    }
    kd_accelerator::node::intermediate_data& kd_accelerator::node::get_intermediate_data() noexcept
    {
#if GTL_CHECK_CAST()
        if (is_leaf())
            throw std::exception();
#endif // GTL_CAST_CHECK
        return *reinterpret_cast<intermediate_data*>(m_storage);
    }
    kd_accelerator::node::intermediate_data const& kd_accelerator::node::get_intermediate_data() const noexcept
    {
#if GTL_CHECK_CAST()
        if (is_leaf())
            throw std::exception();
#endif // GTL_CAST_CHECK
        return *reinterpret_cast<intermediate_data const*>(m_storage);
    }
}


