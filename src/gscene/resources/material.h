#pragma once

#include "gscene/resource.h"

namespace gscene
{
    struct material : resource
    {
        virtual ~material() = 0 {};
        void load() override {}
    };
}