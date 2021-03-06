#pragma once

#include "gmath/ray.h"

#include "grender/integrator.h"

namespace gscene
{
    struct ray_hit;
    struct light;
}

namespace grender
{
    struct blinn_phong_integrator
        : integrator
    {
        void verify_scene(gscene::scene const& scene) const override final;
        void render(gscene::scene const& scene, grender::camera const& camera, glm::uvec2 bounds) const override final;
    private:
        glm::vec3 compute_radiance(gmath::ray<gmath::world_space> const& ray, gscene::scene const& scene, grender::camera const& camera) const noexcept;
        glm::vec3 compute_blinn_phong(gscene::ray_hit const& hit, gscene::light const& light, gscene::scene const& scene, grender::camera const& camera) const noexcept;
    };
}