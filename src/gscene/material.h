#pragma once

namespace gscene
{
    struct material
    {
        virtual ~material() = 0 {};
        virtual std::unique_ptr<material> clone() const = 0;
    };
}