#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::inl::statshouse {

template <bool BOXED>
class SampleFactor_BASE
    : private MAYBE<Magic, BOXED, 2110667114> {
private:
    using m_magic = MAYBE<Magic, BOXED, 2110667114>;

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
        : m_magic(proxy, offset)
        , m_metric(proxy, offset + METRIC_OFFSET)
        , m_value(proxy, offset + VALUE_OFFSET)
    {}

    bool verify() const noexcept
    {
        if constexpr (BOXED)
            if (m_magic::value != MAGIC)
                return false;

        if (!get_metric().verify())
            return false;

        if (!get_value().verify())
            return false;

        return true;
    }

    const Int& get_metric() const noexcept
    {
        return m_metric;
    }

    const Float& get_value() const noexcept
    {
        return m_value;
    }

    static SampleFactor_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Int metric = Int::fetch(stream);
        Float value = Float::fetch(stream);
        SampleFactor_BASE result(std::move(metric),
                                 std::move(value));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_metric.store(stream);
        m_value.store(stream);
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

    SampleFactor_BASE(Int&& metric,
                      Float&& value) noexcept
        : m_magic(MAGIC)
        , m_metric(std::move(metric))
        , m_value(std::move(value))
    {}

    Int m_metric;
    Float m_value;
};

using sampleFactor = SampleFactor_BASE<false>;
using SampleFactor = SampleFactor_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const SampleFactor_BASE<LHS_BOXED>& lhs, const SampleFactor_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_metric() == rhs.get_metric()
           && lhs.get_value() == rhs.get_value();
}

}    // namespace opus::inl::statshouse
