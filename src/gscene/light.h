#pragma once


namespace gscene
{
    struct light
    {
        light(world_transform const& transform) : m_transform(transform) {}
        world_transform const& get_transform() const { return m_transform; }

    private:
        world_transform m_transform;
    };
}