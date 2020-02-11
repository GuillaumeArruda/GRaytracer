#pragma once

#include <vector>
#include <memory>
#include <string_view>
#include <optional>

#include "gtl/span.h"

#include "gmath\ray.h"
#include "gscene\ray_hit.h"
#include "gscene\object.h"

namespace gscene
{
    struct light;
    struct scene
    {
        scene() noexcept = default;
        scene(scene const&) = delete;
        scene(scene&&) noexcept = default;
        ~scene();

        scene& operator=(scene const&) = delete;
        scene& operator=(scene&&) = default;

        std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray) const noexcept;

        gtl::span<const std::unique_ptr<light>> get_lights() const noexcept { return m_lights; }

        template<typename Archive>
        void serialize(Archive& ar)
        {
            ar(CEREAL_NVP(m_objects), CEREAL_NVP(m_lights));
        }

    private:
        std::vector<std::unique_ptr<light>> m_lights;
        std::vector<object> m_objects;
    };
}