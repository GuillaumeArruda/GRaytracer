#pragma once

#include <CImg\CImg.h>

namespace grender
{
    struct sensor
    {
        sensor(std::size_t width, std::size_t height);
        sensor(sensor const&) = delete;
        sensor(sensor&&) noexcept = default;
        ~sensor() = default;

        sensor& operator=(sensor const&) = delete;
        sensor& operator=(sensor&&) = default;

        void set_pixel(std::size_t x_index, std::size_t y_index, float red, float green, float blue) noexcept;

        cimg_library::CImg<float> const& get_image() const noexcept { return m_image; };

        std::size_t get_width() const noexcept { return m_image.width(); }
        std::size_t get_height() const noexcept { return m_image.height(); }

    private:
        cimg_library::CImg<float> m_image;
    };
}