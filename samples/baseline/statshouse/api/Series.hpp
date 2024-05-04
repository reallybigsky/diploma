#pragma once

#include "baseline/Includes.hpp"



namespace baseline::statshouse {

class Series {
public:
    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
    static constexpr Magic MAGIC = 689864296;
    static constexpr bool CTS = false;

    Series() noexcept = default;

    Series(const Series&) noexcept = default;
    Series(Series&&) noexcept = default;
    Series& operator=(const Series&) noexcept = default;
    Series& operator=(Series&&) noexcept = default;

    ~Series() noexcept = default;

    Series(Nat fields_mask,
           const Array<Array<Double>>& series_data,
           const Array<Long>& time) noexcept
        : m_fields_mask(fields_mask)
        , m_series_data(series_data)
        , m_time(time)
    {}

    Nat get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const Array<Array<Double>>& get_series_data() const noexcept
    {
        return m_series_data;
    }

    const Array<Long>& get_time() const noexcept
    {
        return m_time;
    }

    static Series fetch(InputStream& stream)
    {
        Magic magic = Magic::fetch(stream);
        if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);

        Nat fields_mask = Nat::fetch(stream);
        Array<Array<Double>> series_data = Array<Array<Double>>::fetch(stream);
        Array<Long> time = Array<Long>::fetch(stream);
        return {fields_mask,
                series_data,
                time};
    }

    void store(OutputStream& stream) const
    {
        MAGIC.store(stream);
        m_fields_mask.store(stream);
        m_series_data.store(stream);
        m_time.store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const Series& rhs)
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_series_data == rhs.get_series_data()
                   && lhs.b_time == rhs.get_time();
        }

        Builder& set_fields_mask(Nat::Builder value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_series_data(const Array<Array<Double>>::Builder& value) noexcept
        {
            b_series_data = value;
            return *this;
        }

        Builder& set_time(const Array<Long>::Builder& value) noexcept
        {
            b_time = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            MAGIC.store(stream);
            b_fields_mask.store(stream);
            b_series_data.store(stream);
            b_time.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        Array<Array<Double>>::Builder b_series_data;
        Array<Long>::Builder b_time;
    };

private:
    Nat m_fields_mask;
    Array<Array<Double>> m_series_data;
    Array<Long> m_time;
};

}    // namespace baseline::statshouse
