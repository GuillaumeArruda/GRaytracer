#pragma once

namespace gscene
{
    struct resource
    {
        virtual ~resource() = default;
        virtual void load() = 0;
    };
}