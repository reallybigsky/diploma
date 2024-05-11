#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::proxy::statshouse {

template <bool BOXED>
class IngestionStatus2_BASE
    : private MAYBE<Magic, BOXED, 779890644> {
private:
    using m_magic = MAYBE<Magic, BOXED, 779890644>;

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
        : m_magic(proxy, offset)
        , m_env(proxy, offset + ENV_OFFSET)
        , m_metric(proxy, offset + METRIC_OFFSET)
        , m_value(proxy, offset + VALUE_OFFSET)
    {}

    bool verify() const noexcept
    {
        if constexpr (BOXED)
            if (m_magic::value != MAGIC)
                return false;

        if (!get_env().verify())
            return false;

        if (!get_metric().verify())
            return false;

        if (!get_value().verify())
            return false;

        return true;
    }

    const Int& get_env() const noexcept
    {
        return m_env;
    }

    const Int& get_metric() const noexcept
    {
        return m_metric;
    }

    const Float& get_value() const noexcept
    {
        return m_value;
    }

    static IngestionStatus2_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Int env = Int::fetch(stream);
        Int metric = Int::fetch(stream);
        Float value = Float::fetch(stream);
        IngestionStatus2_BASE result(std::move(env),
                                     std::move(metric),
                                     std::move(value));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_env.store(stream);
        m_metric.store(stream);
        m_value.store(stream);
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


    IngestionStatus2_BASE(Int&& env,
                          Int&& metric,
                          Float&& value) noexcept
        : m_magic(MAGIC)
        , m_env(std::move(env))
        , m_metric(std::move(metric))
        , m_value(std::move(value))
    {}

    Int m_env;
    Int m_metric;
    Float m_value;
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

template <bool BOXED>
size_t consume(const IngestionStatus2_BASE<BOXED>& value) noexcept
{
    size_t result = 0;
    result += consume(value.get_env());
    result += consume(value.get_metric());
    result += consume(value.get_value());
    return result;
}

}    // namespace opus::proxy::statshouse
