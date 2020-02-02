#pragma once

#include <memory>

#include "gscene/common.h"

namespace gscene
{
    struct lense;
    struct camera
    {
        camera(world_transform const& transform, std::unique_ptr<lense> lense) noexcept;
        camera(camera const&) = delete;
        camera(camera&&) noexcept = default;
        ~camera() = default;

        camera& operator=(camera const&) = delete;
        camera& operator=(camera&&) noexcept = default;


    private:
        camera_transform m_camera_transform;
        world_transform m_world_transform;
        std::unique_ptr<lense> m_lense;
    };
}