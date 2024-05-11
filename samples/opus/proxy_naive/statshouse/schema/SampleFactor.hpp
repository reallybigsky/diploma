#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::proxy_naive::statshouse {

template <bool BOXED>
class SampleFactor_BASE {
public:
    static constexpr Magic MAGIC = 2110667114;
    static constexpr offset_t SIZEOF = (BOXED ? Magic::SIZEOF : 0)
                                       + Int::SIZEOF
                                       + Float::SIZEOF;
    static constexpr bool STATIC = true;

    SampleFactor_BASE() noexcept = default;

    SampleFactor_BASE(const SampleFactor_BASE&) noexcept = default;
    SampleFactor_BASE(SampleFactor_BASE&&) noexcept = default;
    SampleFactor_BASE& operator=(const SampleFactor_BASE&) noexcept = default;
    SampleFactor_BASE& operator=(SampleFactor_BASE&&) noexcept = default;

    ~SampleFactor_BASE() noexcept = default;

    SampleFactor_BASE(const Proxy& proxy, offset_t offset) noexcept
        : m_proxy(proxy, offset)
    {}

    bool verify() const noexcept
    {
        if constexpr (BOXED)
            if (*(Magic*)(m_proxy.data()) != MAGIC)
                return false;

        if (!get_metric().verify())
            return false;

        if (!get_value().verify())
            return false;

        return true;
    }

    Int get_metric() const noexcept
    {
        return Int {m_proxy, METRIC_OFFSET};
    }

    Float get_value() const noexcept
    {
        return Float {m_proxy, VALUE_OFFSET};
    }

    static SampleFactor_BASE fetch(InputStream& stream)
    {
        Proxy proxy = Proxy::fetch(stream, SIZEOF);
        SampleFactor_BASE result(std::move(proxy));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_proxy.store(stream, SIZEOF);
    }

    class Builder {
    public:
        using TYPE = SampleFactor_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const SampleFactor_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_metric == rhs.get_metric()
                   && lhs.b_value == rhs.get_value();
        }

        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_metric(Int::Builder::random(engine))
                    .set_value(Float::Builder::random(engine));
        }

        Builder& set_metric(const Int::Builder& value) noexcept
        {
            b_metric = value;
            return *this;
        }

        Builder& set_value(const Float::Builder& value) noexcept
        {
            b_value = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_metric.store(stream);
            b_value.store(stream);
        }

    private:
        Int::Builder b_metric;
        Float::Builder b_value;
    };

private:
    static constexpr offset_t METRIC_OFFSET = BOXED ? Magic::SIZEOF : 0;
    static constexpr offset_t VALUE_OFFSET = METRIC_OFFSET + Int::SIZEOF;

    SampleFactor_BASE(Proxy&& proxy) noexcept
        : m_proxy(std::move(proxy))
    {}

    Proxy m_proxy;
};

using sampleFactor = SampleFactor_BASE<false>;
using SampleFactor = SampleFactor_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const SampleFactor_BASE<LHS_BOXED>& lhs, const SampleFactor_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_metric() == rhs.get_metric()
           && lhs.get_value() == rhs.get_value();
}

template <bool BOXED>
size_t consume(const SampleFactor_BASE<BOXED>& value) noexcept
{
    size_t result = 0;
    result += consume(value.get_metric());
    result += consume(value.get_value());
    return result;
}

}    // namespace opus::proxy_naive::statshouse
