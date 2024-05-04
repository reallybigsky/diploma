#pragma once

#include "baseline/Includes.hpp"



namespace baseline::statshouse {

class Centroid {
public:
    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
    static constexpr bool CTS = true;

    Centroid() noexcept = default;

    Centroid(const Centroid&) noexcept = default;
    Centroid(Centroid&&) noexcept = default;
    Centroid& operator=(const Centroid&) noexcept = default;
    Centroid& operator=(Centroid&&) noexcept = default;

    ~Centroid() noexcept = default;

    Centroid(Float value,
             Float weight) noexcept
        : m_value(value)
        , m_weight(weight)
    {}

    Float get_value() const noexcept
    {
        return m_value;
    }

    Float get_weight() const noexcept
    {
        return m_weight;
    }

    static Centroid fetch(InputStream& stream)
    {
        Float value = Float::fetch(stream);
        Float weight = Float::fetch(stream);
        return {value,
                weight};
    }

    void store(OutputStream& stream) const
    {
        m_value.store(stream);
        m_weight.store(stream);
    }

    class Builder {
    public:
        Builder& set_value(Float::Builder value) noexcept
        {
            b_value = value;
            return *this;
        }

        Builder& set_weight(Float::Builder value) noexcept
        {
            b_weight = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            b_value.store(stream);
            b_weight.store(stream);
        }

    private:
        Float::Builder b_value;
        Float::Builder b_weight;
    };

private:
    Float m_value;
    Float m_weight;
};

}    // namespace baseline::statshouse
