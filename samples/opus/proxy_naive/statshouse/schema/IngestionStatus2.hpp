#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::proxy_naive::statshouse {

template <bool BOXED>
class IngestionStatus2_BASE {
public:
    static constexpr Magic MAGIC = 779890644;
    static constexpr offset_t SIZEOF = (BOXED ? Magic::SIZEOF : 0)
                                       + Int::SIZEOF
                                       + Int::SIZEOF
                                       + Float::SIZEOF;
    static constexpr bool STATIC = true;

    IngestionStatus2_BASE() noexcept = default;

    IngestionStatus2_BASE(const IngestionStatus2_BASE&) noexcept = default;
    IngestionStatus2_BASE(IngestionStatus2_BASE&&) noexcept = default;
    IngestionStatus2_BASE& operator=(const IngestionStatus2_BASE&) noexcept = default;
    IngestionStatus2_BASE& operator=(IngestionStatus2_BASE&&) noexcept = default;

    ~IngestionStatus2_BASE() noexcept = default;

    IngestionStatus2_BASE(const Proxy& proxy, offset_t offset) noexcept
        : m_proxy(proxy, offset)
    {}

    bool verify() const noexcept
    {
        if constexpr (BOXED)
            if (*(Magic*)(m_proxy.data()) != MAGIC)
                return false;

        if (!get_env().verify())
            return false;

        if (!get_metric().verify())
            return false;

        if (!get_value().verify())
            return false;

        return true;
    }

    Int get_env() const noexcept
    {
        return Int {m_proxy, ENV_OFFSET};
    }

    Int get_metric() const noexcept
    {
        return Int {m_proxy, METRIC_OFFSET};
    }

    Float get_value() const noexcept
    {
        return Float {m_proxy, VALUE_OFFSET};
    }

    static IngestionStatus2_BASE fetch(InputStream& stream)
    {
        Proxy proxy = Proxy::fetch(stream, SIZEOF);
        IngestionStatus2_BASE result(std::move(proxy));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_proxy.store(stream, SIZEOF);
    }

    class Builder {
    public:
        using TYPE = IngestionStatus2_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const IngestionStatus2_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_env == rhs.get_env()
                   && lhs.b_metric == rhs.get_metric()
                   && lhs.b_value == rhs.get_value();
        }

        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_env(Int::Builder::random(engine))
                    .set_metric(Int::Builder::random(engine))
                    .set_value(Float::Builder::random(engine));
        }

        Builder& set_env(const Int::Builder& value) noexcept
        {
            b_env = value;
            return *this;
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
            b_env.store(stream);
            b_metric.store(stream);
            b_value.store(stream);
        }

    private:
        Int::Builder b_env;
        Int::Builder b_metric;
        Float::Builder b_value;
    };

private:
    static constexpr offset_t ENV_OFFSET = BOXED ? Magic::SIZEOF : 0;
    static constexpr offset_t METRIC_OFFSET = ENV_OFFSET + Int::SIZEOF;
    static constexpr offset_t VALUE_OFFSET = METRIC_OFFSET + Int::SIZEOF;

    IngestionStatus2_BASE(Proxy&& proxy) noexcept
        : m_proxy(std::move(proxy))
    {}

    Proxy m_proxy;
};

using ingestionStatus2 = IngestionStatus2_BASE<false>;
using IngestionStatus2 = IngestionStatus2_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const IngestionStatus2_BASE<LHS_BOXED>& lhs, const IngestionStatus2_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_env() == rhs.get_env()
           && lhs.get_metric() == rhs.get_metric()
           && lhs.get_value() == rhs.get_value();
}

}    // namespace opus::proxy_naive::statshouse
