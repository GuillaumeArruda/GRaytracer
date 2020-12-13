#pragma once

#include <glm/glm.hpp>
#include "gscene/resources/material.h"

namespace gscene
{
    struct blinn_phong_material
        : material
    {
        void process(gserializer::serializer& serializer) override;
        glm::vec3 m_diffuse_color = {};
        glm::vec3 m_specular_color = {};
        float m_specular_exponent = 1.f;
        float m_reflection_coefficient = 0.f;
        float m_refraction_coefficient = 0.f;
        GSERIALIZER_DECLARE_SUBCLASS_FACTORY_REGISTRATION();
    };
}