#pragma once

#include <vector>
#include <memory>
#include <string_view>
#include <optional>

#include "gtl/span.h"

#include "gmath/ray.h"

#include "gscene/ray_hit.h"
#include "gscene/object.h"
#include "gscene/accelerator.h"
#include "gscene/resource_library.h"

namespace gthread
{
    struct job_manager;
}

namespace gscene
{
    struct light;
    struct scene
    {
        scene() = default;
        scene(scene const&) = delete;
        scene(scene&&) noexcept = default;
        ~scene();

        scene& operator=(scene const&) = delete;
        scene& operator=(scene&&) = default;

        std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray) const noexcept;

        gtl::span<std::unique_ptr<light> const> get_lights() const noexcept { return m_lights; }
        gtl::span<object const> get_objects() const noexcept { return m_objects; }

        void prepare_scene(gthread::job_manager& jobs);

        template<typename Archive>
        void serialize(Archive& ar)
        {
            ar(CEREAL_NVP(m_resource_library), CEREAL_NVP(m_objects), CEREAL_NVP(m_lights), CEREAL_NVP(m_accelerator));
        }


    private:
        resource_library m_resource_library;
        std::vector<std::unique_ptr<light>> m_lights;
        std::vector<object> m_objects;
        std::unique_ptr<accelerator> m_accelerator;
    };
}