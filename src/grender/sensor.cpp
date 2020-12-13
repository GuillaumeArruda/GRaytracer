#include "stdafx.h"
#include "grender/sensor.h"

namespace grender
{
    sensor::sensor(std::size_t width, std::size_t height) : m_width(width), m_height(height)
    {
    }
    void sensor::set_pixel(std::size_t x_index, std::size_t y_index, float red, float green, float blue) noexcept
    {
    }
}

