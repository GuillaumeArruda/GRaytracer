#include "stdafx.h"
#include "gscene/light.h"

#include "gserializer/serializer.h"

namespace gscene
{
    void light::process(gserializer::serializer& serializer)
    {
        serializer.process("m_transform", m_transform);
        serializer.process("m_color", m_color);
    }
}

