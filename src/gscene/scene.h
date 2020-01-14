#pragma once

#include <vector>
#include <memory>

namespace gscene
{
    struct light;
    struct aggregator;
    struct scene
    {

    private:
        std::vector<std::unique_ptr<light>> m_lights;
    };
}