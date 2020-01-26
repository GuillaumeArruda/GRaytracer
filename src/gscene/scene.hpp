#pragma once

#include <vector>
#include <memory>
#include <string_view>
#include <optional>

#include "gmath\ray.hpp"
#include "gscene\ray_hit.hpp"
#include "gscene\object.hpp"

namespace gscene
{
    struct light;
    struct scene
    {
        scene(std::string_view filename);
        scene(scene const&) = delete;
        scene(scene&&) noexcept = default;
        ~scene();

        scene& operator=(scene const&) = delete;
        scene& operator=(scene&&) = default;

        std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> ray) const noexcept;

    private:
        std::vector<std::unique_ptr<light>> m_lights;
        std::vector<object> m_objects;
    };
}