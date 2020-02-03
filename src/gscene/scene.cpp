#include "gscene\scene.h"
#include "gscene\light.h"
#include "gscene\material.h"
#include "gscene\shape.h"

#include "gscene\shapes\sphere.h"
#include "gscene\shapes\plane.h"

#include <glm/gtc/matrix_transform.hpp>

namespace gscene
{
    scene::~scene() = default;

    scene::scene(std::string_view filename)
    {
        m_objects.emplace_back(world_transform(glm::translate(glm::mat4(1.f), glm::vec3(0.5f, 0.f, 2.f))), std::make_unique<sphere>(0.4f), std::make_unique<material>());
        m_objects.emplace_back(world_transform(glm::translate(glm::mat4(1.f), glm::vec3(-1.f, 1.f, 1.f))), std::make_unique<sphere>(0.2f), std::make_unique<material>());
        m_objects.emplace_back(world_transform(glm::translate(glm::mat4(1.f), glm::vec3(3.f, 3.f, 15.f))), std::make_unique<sphere>(1.f), std::make_unique<material>());
        m_objects.emplace_back(world_transform(glm::translate(glm::mat4(1.f), glm::vec3(-4.f, 1.f, 20.f))), std::make_unique<sphere>(2.f), std::make_unique<material>());
        m_objects.emplace_back(world_transform(glm::translate(glm::mat4(1.f), glm::vec3(0.f, -10.f, 0.f))), std::make_unique<plane>(gmath::direction<gmath::world_space>(0.f, 1.f, 0.f)), std::make_unique<material>());

        m_lights.emplace_back(std::make_unique<light>(world_transform(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 40.f, 10.f)))));
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
