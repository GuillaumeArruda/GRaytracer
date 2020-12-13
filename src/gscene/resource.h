#pragma once

#include "gserializer/type_factory.h"

namespace gserializer
{
    struct serializer;
}

namespace gscene
{
    struct resource
    {
        virtual ~resource() = default;
        virtual void load() = 0;
        virtual void process(gserializer::serializer& serializer) = 0;
        GSERIALIZER_DECLARE_FACTORY_BASE(resource);
    };
}