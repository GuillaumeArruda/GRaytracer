#pragma once

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

        std::size_t get_width() const noexcept { return m_width; }
        std::size_t get_height() const noexcept { return m_height; }

    private:
        std::size_t m_width;
        std::size_t m_height;
    };
}