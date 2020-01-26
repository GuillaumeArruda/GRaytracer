#include "gscene\shapes\sphere.hpp"
#include "gscene\material.hpp"
#include "gscene\object.hpp"

namespace gscene
{
    std::optional<ray_hit> sphere::raycast(gmath::ray<gmath::world_space> const& ray, object const& obj) const noexcept
    {
        static const material m;
        const gmath::ray<gmath::model_space> local_ray = obj.get_inverse_transform() * ray;

        float const a = local_ray.dir().length2();
        float const b = 2.f * (local_ray.dir().x() * local_ray.start().x() + local_ray.dir().y() * local_ray.start().y() + local_ray.dir().z() * local_ray.start().z());
        float const c = (local_ray.start() - gmath::position<gmath::model_space>()).length2() - (m_radius * m_radius);

        if (std::optional<gmath::quadatric_result> quad_coefficient = gmath::quadatric(a, b, c))
        {
            if (quad_coefficient->m_first_t <= local_ray.maxT())
            {
                std::optional<float> t;
                if (quad_coefficient->m_first_t >= local_ray.minT())
                    t = quad_coefficient->m_first_t;
                else if (quad_coefficient->m_second_t >= local_ray.minT() && quad_coefficient->m_second_t <= local_ray.maxT())
                    t = quad_coefficient->m_second_t;
                
                if (!t)
                    return {};

                gmath::position<gmath::world_space> const hitpos = ray(*t);
                gmath::direction<gmath::world_space> const normal = hitpos - obj.get_transform().get_translation();
                return ray_hit{ hitpos, normal, &obj, *t };
            }
        }
        return {};
    }
}
