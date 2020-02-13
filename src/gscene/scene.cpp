#include "stdafx.h"

#include <glm/gtc/matrix_transform.hpp>

#include "gscene/scene.h"
#include "gscene/light.h"


namespace gscene
{
    scene::~scene() = default;

    std::optional<ray_hit> scene::raycast(gmath::ray<gmath::world_space> const& ray) const noexcept
    {
        std::optional<ray_hit> besthit;
        for (auto const& object: m_objects)
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

}
