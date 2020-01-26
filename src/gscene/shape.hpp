#pragma once

#include <optional>

#include "gmath\transform.hpp"
#include "gmath\ray.hpp"

#include "gscene\material.hpp"
#include "gscene\ray_hit.hpp"
#include "gscene\common.hpp"
namespace gscene
{
    struct object;
    struct shape
    {
        virtual ~shape() {};

        virtual std::optional<ray_hit> raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept = 0;

    protected:
        shape() noexcept = default;
        shape(shape const&) noexcept = default;
        shape(shape&&) noexcept = default;
        shape& operator=(shape const&) noexcept = default;
        shape& operator=(shape&&) noexcept = default;
    };
}