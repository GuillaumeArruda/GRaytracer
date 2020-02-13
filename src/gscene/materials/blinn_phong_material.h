#pragma once

#include <glm/glm.hpp>

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>

#include "gscene/material.h"

namespace gscene
{
    struct blinn_phong_material
        : material
    {
        template<class Archive>
        void serialize(Archive& ar)
        {
            ar(CEREAL_NVP(m_diffuse_color), CEREAL_NVP(m_specular_color), CEREAL_NVP(m_specular_exponent));
        }
        glm::vec3 m_diffuse_color;
        glm::vec3 m_specular_color;
        float m_specular_exponent;
    };
}

CEREAL_REGISTER_TYPE(gscene::blinn_phong_material);
CEREAL_REGISTER_POLYMORPHIC_RELATION(gscene::material, gscene::blinn_phong_material);