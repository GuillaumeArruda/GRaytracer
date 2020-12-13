#pragma once

#include <glm/glm.hpp>

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>

#include "gscene/resources/material.h"

namespace gscene
{
    struct blinn_phong_material
        : material
    {
        template<class Archive>
        void serialize(Archive& ar)
        {
            ar(CEREAL_NVP(m_diffuse_color)
            , CEREAL_NVP(m_specular_color)
            , CEREAL_NVP(m_specular_exponent)
            , CEREAL_NVP(m_reflection_coefficient)
            , CEREAL_NVP(m_refraction_coefficient));
        }

        void process(gserializer::serializer& serializer) override;
        glm::vec3 m_diffuse_color = {};
        glm::vec3 m_specular_color = {};
        float m_specular_exponent = 1.f;
        float m_reflection_coefficient = 0.f;
        float m_refraction_coefficient = 0.f;
        GSERIALIZER_DECLARE_SUBCLASS_FACTORY_REGISTRATION();
    };
}

CEREAL_REGISTER_TYPE(gscene::blinn_phong_material);
CEREAL_REGISTER_POLYMORPHIC_RELATION(gscene::resource, gscene::blinn_phong_material);