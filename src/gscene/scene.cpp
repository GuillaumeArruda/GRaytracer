#include "gscene\scene.hpp"
#include "gscene\light.hpp"
#include "gscene\material.hpp"
#include "gscene\shape.hpp"

#include "gscene\shapes\sphere.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace gscene
{
    scene::~scene() = default;

    scene::scene(std::string_view filename)
    {
        m_objects.emplace_back(world_transform(glm::translate(glm::mat4(1.f), glm::vec3(40.f, 40.f, 10.f))), std::make_unique<sphere>(10.f), std::make_unique<material>());
    }

    std::optional<ray_hit> scene::raycast(gmath::ray<gmath::world_space> ray) const noexcept
    {
        std::optional<ray_hit> besthit;
        for (auto const& object: m_objects)
        {
            std::optional<ray_hit> hit = object.raycast(ray);
            if (hit)
                if (!besthit)
                    besthit = std::move(hit);
                else
                    if (*besthit < *hit)
                        besthit = std::move(hit);
        }
        return besthit;
    }

}
