#pragma once


#include "gmath/position.h"
#include "gmath/direction.h"
#include "gmath/transform.h"
#include "gmath/units.h"

#include "gserializer/serializer.h"

namespace gserializer
{
    inline void process(serializer& serializer, glm::vec3& vector)
    {
        serializer.process("x", vector.x);
        serializer.process("y", vector.y);
        serializer.process("z", vector.z);
    }

    inline void process(serializer& serializer, glm::vec4& vector)
    {
        serializer.process("x", vector.x);
        serializer.process("y", vector.y);
        serializer.process("z", vector.z);
        serializer.process("w", vector.w);
    }

    inline void process(serializer& serializer, glm::mat4& matrix)
    {
        serializer.process("[0]", matrix[0]);
        serializer.process("[1]", matrix[1]);
        serializer.process("[2]", matrix[2]);
        serializer.process("[3]", matrix[3]);
    }

    inline void process(serializer& serializer, gmath::radian& radian)
    {
        serializer.process("m_value", radian.value());
    }

    inline void process(serializer& serializer, gmath::degree& degree)
    {
        serializer.process("m_value", degree.value());
    }

    template<class Space>
    void process(serializer& serializer, gmath::position<Space>& position)
    {
        serializer.process("m_position", position.get_internal_position());
    }

    template<class Space>
    void process(serializer& serializer, gmath::direction<Space>& direction)
    {
        auto dir = static_cast<typename gmath::direction<Space>::glm_vector_type>(direction);
        serializer.process("x", dir);
        direction = gmath::direction<Space>(dir);
    }

    template<class SpaceFrom, class SpaceTo>
    void process(serializer& serializer, gmath::transform<SpaceFrom, SpaceTo>& transform)
    {
        serializer.process("m_transform", transform.get_internal_matrix());
    }
}