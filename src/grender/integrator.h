#pragma once

#include <glm\glm.hpp>

namespace gscene
{
    struct scene;
}
namespace grender
{
    struct camera;
    struct integrator
    {
        virtual ~integrator() = 0 {};

        virtual void verify_scene(gscene::scene const& scene) const = 0;
        virtual void render(gscene::scene const& scene, grender::camera const& camera, glm::uvec2 bounds) const = 0 ;
    };
}