#include "stdafx.h"
#include "gscene/resources/materials/blinn_phong_material.h"

#include "gserializer/serializer.h"
#include "gserializer/gmath_serialization.h"

GSERIALIZER_DEFINE_SUBCLASS_FACTORY_REGISTRATION(gscene::blinn_phong_material);

namespace gscene
{
    void blinn_phong_material::process(gserializer::serializer& serializer)
    {
        serializer.process("m_diffuse_color", m_diffuse_color);
        serializer.process("m_specular_color", m_specular_color);
        serializer.process("m_specular_exponent", m_specular_exponent);
        serializer.process("m_reflection_coefficient", m_reflection_coefficient);
        serializer.process("m_refraction_coefficient", m_refraction_coefficient);
    }
}

