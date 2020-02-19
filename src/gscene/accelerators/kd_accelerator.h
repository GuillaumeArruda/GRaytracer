#pragma once
#include <type_traits>

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>

#include "gtl/span.h"

#include "gmath/axis_aligned_box.h"

#include "gscene/accelerator.h"



namespace gscene
{
    struct kd_accelerator final : accelerator
    {

        kd_accelerator() noexcept;
        ~kd_accelerator() = default;
        kd_accelerator(kd_accelerator const&) = default;
        kd_accelerator(kd_accelerator&&) noexcept = default;

        kd_accelerator& operator=(kd_accelerator const&) = default;
        kd_accelerator& operator=(kd_accelerator&&) = default;


        virtual std::optional<gscene::ray_hit> raycast(gmath::ray<gmath::world_space> const& ray) const noexcept final;
        virtual void build(gscene::scene const& scene) final;

        template<class Archive>
        void serialize(Archive& ar) {}
    private:
        struct bound_edge
        {
            std::uint32_t m_object_index;
            float m_t;
            enum class type
            {
                begin,
                end,
            };
            type m_type;

        };

        struct node
        {
            enum class split_axis : unsigned char
            {
                x = 0,
                y = 1,
                z = 2
            };


            node(std::unique_ptr<object const* []> objects, std::uint32_t size);
            node(split_axis axis, float split_position);
            ~node();
            node(node const&) = delete;
            node(node&&) noexcept;

            node& operator=(node const&) = delete;
            node& operator=(node&&) noexcept;

            struct node_data
            {
                bool m_is_leaf : 1;
            };

            struct alignas(8) leaf_data : node_data
            {
                std::uint32_t m_number_of_objects : 31;
                std::unique_ptr<object const* []> m_objects;
            };
            struct alignas(8) intermediate_data : node_data
            { 
                split_axis m_split_axis : 2;
                std::uint32_t m_right_child_index : 29;
                float m_split_position;
            };
             
            bool is_leaf() const noexcept;
            leaf_data& get_leaf_data() noexcept;
            leaf_data const& get_leaf_data() const noexcept;
            intermediate_data& get_intermediate_data() noexcept;
            intermediate_data const& get_intermediate_data() const noexcept;

            template<class Archive>
            void serialize(Archive& ar)
            {
                ar(CEREAL_NVP(m_max_depth), CEREAL_NVP(m_max_cost), CEREAL_NVP(m_isect_cost), CEREAL_NVP(m_empty_bonus), CEREAL_NVP(m_traversal_cost));
            }
        private:
            static_assert(alignof(leaf_data) == alignof(intermediate_data));
            alignas(8) unsigned char m_storage[std::max(sizeof(leaf_data), sizeof(intermediate_data))];
        };

        void recursive_build(gtl::span<object const> all_object, gmath::axis_aligned_box<gmath::world_space> const& node_bound
            , std::vector<gmath::axis_aligned_box<gmath::world_space>> const& all_objects_bounds
            , gtl::span<std::size_t const> objects_idx, std::uint32_t depth, std::array<std::vector<bound_edge>, 3>& edges);

        void create_leaf(gtl::span<object const> all_objects, gtl::span<std::size_t const> indexes);

        std::vector<node> m_nodes;
        gmath::axis_aligned_box<gmath::world_space> m_bound;
        std::uint32_t m_max_depth = 4;
        std::uint32_t m_max_cost = 4;
        float m_isect_cost = 80.f;
        float m_empty_bonus = 0.5f; 
        float m_traversal_cost = 1.f;
    };
}

CEREAL_REGISTER_TYPE(gscene::kd_accelerator);
CEREAL_REGISTER_POLYMORPHIC_RELATION(gscene::accelerator, gscene::kd_accelerator);