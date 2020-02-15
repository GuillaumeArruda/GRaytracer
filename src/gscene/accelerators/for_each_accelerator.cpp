#include "stdafx.h"

#include "gscene/accelerators/for_each_accelerator.h"

#include "gscene/scene.h"

namespace gscene
{
    std::optional<gscene::ray_hit> for_each_accelerator::raycast(gmath::ray<gmath::world_space> const& ray) const noexcept
    {
        std::optional<ray_hit> besthit;
        for (auto const& object : m_objects)
        {
            std::optional<ray_hit> hit = object.raycast(ray);
            if (hit)
                if (!besthit)
                    besthit = std::move(hit);
                else
                    if (*hit < *besthit)
                        besthit = std::move(hit);
        }
        return besthit;
    }

    void for_each_accelerator::build(scene const& scene)
    {
        m_objects = scene.get_objects();
    }
}
