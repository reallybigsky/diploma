#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::inl::statshouse {

template <bool BOXED>
class Series_BASE {
public:
    static constexpr Magic MAGIC = 689864296;
    static constexpr bool STATIC = false;

    Series_BASE() noexcept = default;

    Series_BASE(const Series_BASE&) noexcept = default;
    Series_BASE(Series_BASE&&) noexcept = default;
    Series_BASE& operator=(const Series_BASE&) noexcept = default;
    Series_BASE& operator=(Series_BASE&&) noexcept = default;

    ~Series_BASE() noexcept = default;

    const Nat& get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const array<array<Double>>& get_series_data() const noexcept
    {
        return m_series_data;
    }

    const array<Long>& get_time() const noexcept
    {
        return m_time;
    }

    static Series_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Nat fields_mask = Nat::fetch(stream);
        array<array<Double>> series_data = array<array<Double>>::fetch(stream);
        array<Long> time = array<Long>::fetch(stream);
        Series_BASE result(std::move(fields_mask),
                           std::move(series_data),
                           std::move(time));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_fields_mask.store(stream);
        m_series_data.store(stream);
        m_time.store(stream);
    }

    class Builder {
    public:
        using TYPE = Series_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const Series_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_series_data == rhs.get_series_data()
                   && lhs.b_time == rhs.get_time();
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(Nat::Builder::random(engine))
                    .set_series_data(array<array<Double>>::Builder::random<SIZE_1, SIZE_2>(engine))
                    .set_time(array<Long>::Builder::random<SIZE_3>(engine));
        }

        Builder& set_fields_mask(const Nat::Builder& value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_series_data(const array<array<Double>>::Builder& value) noexcept
        {
            b_series_data = value;
            return *this;
        }

        Builder& set_time(const array<Long>::Builder& value) noexcept
        {
            b_time = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_fields_mask.store(stream);
            b_series_data.store(stream);
            b_time.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        array<array<Double>>::Builder b_series_data;
        array<Long>::Builder b_time;
    };

private:
    Series_BASE(Nat&& fields_mask,
                array<array<Double>>&& series_data,
                array<Long>&& time) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_series_data(std::move(series_data))
        , m_time(std::move(time))
    {}

    Nat m_fields_mask;
    array<array<Double>> m_series_data;
    array<Long> m_time;
};

using series = Series_BASE<false>;
using Series = Series_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const Series_BASE<LHS_BOXED>& lhs, const Series_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_series_data() == rhs.get_series_data()
           && lhs.get_time() == rhs.get_time();
}

}    // namespace opus::inl::statshouse
