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

        gtl::span<std::unique_ptr<light> const> get_lights() const noexcept { return m_lights; }
        gtl::span<object const> get_objects() const noexcept { return m_objects; }

        template<typename Archive>
        void save(Archive& ar) const
        {
            ar(CEREAL_NVP(m_objects), CEREAL_NVP(m_lights), CEREAL_NVP(m_accelerator));
        }

        template<typename Archive>
        void load(Archive& ar)
        {
            ar(CEREAL_NVP(m_objects), CEREAL_NVP(m_lights), CEREAL_NVP(m_accelerator));
            m_accelerator->build(*this);
        }

    private:
        std::vector<std::unique_ptr<light>> m_lights;
        std::vector<object> m_objects;
        std::unique_ptr<accelerator> m_accelerator;
    };
}