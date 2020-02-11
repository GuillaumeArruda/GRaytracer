#pragma once


namespace gscene
{
    struct light
    {
        light(world_transform const& transform) : m_transform(transform) {}
        light() noexcept = default;

        world_transform const& get_transform() const { return m_transform; }

        template<typename Archive>
        void serialize(Archive& ar)
        {
            ar(CEREAL_NVP(m_transform));
        }

    private:
        world_transform m_transform;
    };
}