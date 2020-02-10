#include "grender/sensor.h"

namespace grender
{
    sensor::sensor(std::size_t width, std::size_t height)
        : m_image(static_cast<unsigned int>(width), static_cast<unsigned int>(height), 1, 3, 0.f)
    {
    }
    void sensor::set_pixel(std::size_t x_index, std::size_t y_index, float red, float green, float blue) noexcept
    {
        std::size_t const resolution = get_width() * get_height();
        std::size_t const red_index = (x_index + y_index * get_width());
        std::size_t const green_index = red_index + resolution;
        std::size_t const blue_index = green_index + resolution;
        m_image[red_index] += red;
        m_image[green_index] += green;
        m_image[blue_index] += blue;
    }
}

