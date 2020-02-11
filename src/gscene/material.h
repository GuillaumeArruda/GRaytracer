#pragma once

namespace gscene
{
    struct material
    {
        glm::vec3 m_color;
        float m_specular_exponent;

        template<typename Archive>
        void serialize(Archive& ar)
        {
            ar(CEREAL_NVP(m_color), CEREAL_NVP(m_specular_exponent));
        }
    };
}