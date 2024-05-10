#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::proxy_naive::statshouse {

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

    bool verify() const noexcept
    {
        if (!get_fields_mask().verify())
            return false;

        return true;
    }

    Nat get_fields_mask() const noexcept
    {
        return Nat {m_proxy_1, PROXY_1_FIELDS_MASK_OFFSET};
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
        Proxy proxy_1 = Proxy::fetch(stream, PROXY_1_SIZEOF);
        array<array<Double>> series_data = array<array<Double>>::fetch(stream);
        array<Long> time = array<Long>::fetch(stream);
        Series_BASE result(std::move(proxy_1),
                           std::move(series_data),
                           std::move(time));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_proxy_1.store(stream, PROXY_1_SIZEOF);
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
    static constexpr offset_t PROXY_1_SIZEOF = Nat::SIZEOF;
    static constexpr offset_t PROXY_1_FIELDS_MASK_OFFSET = 0;

    Series_BASE(Proxy&& proxy,
                array<array<Double>>&& series_data,
                array<Long>&& time) noexcept
        : m_proxy_1(std::move(proxy))
        , m_series_data(std::move(series_data))
        , m_time(std::move(time))
    {}

    Proxy m_proxy_1;
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

}    // namespace opus::proxy_naive::statshouse
