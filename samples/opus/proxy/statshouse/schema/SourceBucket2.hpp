#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/proxy/statshouse/schema/IngestionStatus2.hpp"
#include "samples/opus/proxy/statshouse/schema/MultiItem.hpp"
#include "samples/opus/proxy/statshouse/schema/SampleFactor.hpp"



namespace opus::proxy::statshouse {

class SourceBucket2 {
public:
    static constexpr Magic MAGIC = 378424531;

    SourceBucket2() noexcept = default;

    SourceBucket2(const SourceBucket2&) noexcept = default;
    SourceBucket2(SourceBucket2&&) noexcept = default;
    SourceBucket2& operator=(const SourceBucket2&) noexcept = default;
    SourceBucket2& operator=(SourceBucket2&&) noexcept = default;

    ~SourceBucket2() noexcept = default;

    bool verify() const noexcept
    {
        if (!get_sample_factors().verify())
            return false;

        if (!get_legacy_agent_env().verify())
            return false;

        return true;
    }

    const array<multiItem>& get_metrics() const noexcept
    {
        return m_metrics;
    }

    const array<sampleFactor>& get_sample_factors() const noexcept
    {
        return m_sample_factors;
    }

    const array<sampleFactor>& get_ingestion_status_ok() const noexcept
    {
        return m_ingestion_status_ok;
    }

    Nat get_missed_seconds() const noexcept
    {
        return m_missed_seconds;
    }

    Int get_legacy_agent_env() const noexcept
    {
        return m_legacy_agent_env;
    }

    const array<ingestionStatus2>& get_ingestion_status_ok2() const noexcept
    {
        return m_ingestion_status_ok2;
    }

    static SourceBucket2 fetch(InputStream& stream)
    {
        array<multiItem> metrics = array<multiItem>::fetch(stream);
        array<sampleFactor> sample_factors = array<sampleFactor>::fetch(stream);
        array<sampleFactor> ingestion_status_ok = array<sampleFactor>::fetch(stream);
        Nat missed_seconds = Nat::fetch(stream);
        Int legacy_agent_env = Int::fetch(stream);
        array<ingestionStatus2> ingestion_status_ok2 = array<ingestionStatus2>::fetch(stream);
        SourceBucket2 result(std::move(metrics),
                             std::move(sample_factors),
                             std::move(ingestion_status_ok),
                             std::move(missed_seconds),
                             std::move(legacy_agent_env),
                             std::move(ingestion_status_ok2));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_metrics.store(stream);
        m_sample_factors.store(stream);
        m_ingestion_status_ok.store(stream);
        m_missed_seconds.store(stream);
        m_legacy_agent_env.store(stream);
        m_ingestion_status_ok2.store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const SourceBucket2& rhs) noexcept
        {
            return lhs.b_metrics == rhs.get_metrics()
                   && lhs.b_sample_factors == rhs.get_sample_factors()
                   && lhs.b_ingestion_status_ok == rhs.get_ingestion_status_ok()
                   && lhs.b_missed_seconds == rhs.get_missed_seconds()
                   && lhs.b_legacy_agent_env == rhs.get_legacy_agent_env()
                   && lhs.b_ingestion_status_ok2 == rhs.get_ingestion_status_ok2();
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3, size_t SIZE_4, size_t SIZE_5, size_t SIZE_6, size_t SIZE_7, size_t SIZE_8, size_t SIZE_9, size_t SIZE_10, size_t SIZE_11>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_metrics(array<multiItem>::Builder::random<SIZE_1, SIZE_2, SIZE_3, SIZE_4, SIZE_5, SIZE_6, SIZE_7, SIZE_8>(engine))
                    .set_sample_factors(array<sampleFactor>::Builder::random<SIZE_9>(engine))
                    .set_ingestion_status_ok(array<sampleFactor>::Builder::random<SIZE_10>(engine))
                    .set_missed_seconds(Nat::Builder::random(engine))
                    .set_legacy_agent_env(Int::Builder::random(engine))
                    .set_ingestion_status_ok2(array<ingestionStatus2>::Builder::random<SIZE_11>(engine));
        }

        Builder& set_metrics(const array<multiItem>::Builder& value) noexcept
        {
            b_metrics = value;
            return *this;
        }

        Builder& set_sample_factors(const array<sampleFactor>::Builder& value) noexcept
        {
            b_sample_factors = value;
            return *this;
        }

        Builder& set_ingestion_status_ok(const array<sampleFactor>::Builder& value) noexcept
        {
            b_ingestion_status_ok = value;
            return *this;
        }

        Builder& set_missed_seconds(const Nat::Builder& value) noexcept
        {
            b_missed_seconds = value;
            return *this;
        }

        Builder& set_legacy_agent_env(const Int::Builder& value) noexcept
        {
            b_legacy_agent_env = value;
            return *this;
        }

        Builder& set_ingestion_status_ok2(const array<ingestionStatus2>::Builder& value) noexcept
        {
            b_ingestion_status_ok2 = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            MAGIC.store(stream);
            b_metrics.store(stream);
            b_sample_factors.store(stream);
            b_ingestion_status_ok.store(stream);
            b_missed_seconds.store(stream);
            b_legacy_agent_env.store(stream);
            b_ingestion_status_ok2.store(stream);
        }

    private:
        array<multiItem>::Builder b_metrics;
        array<sampleFactor>::Builder b_sample_factors;
        array<sampleFactor>::Builder b_ingestion_status_ok;
        Nat::Builder b_missed_seconds;
        Int::Builder b_legacy_agent_env;
        array<ingestionStatus2>::Builder b_ingestion_status_ok2;
    };

private:
    SourceBucket2(array<multiItem>&& metrics,
                  array<sampleFactor>&& sample_factors,
                  array<sampleFactor>&& ingestion_status_ok,
                  Nat&& missed_seconds,
                  Int&& legacy_agent_env,
                  array<ingestionStatus2>&& ingestion_status_ok2) noexcept
        : m_metrics(std::move(metrics))
        , m_sample_factors(std::move(sample_factors))
        , m_ingestion_status_ok(std::move(ingestion_status_ok))
        , m_missed_seconds(std::move(missed_seconds))
        , m_legacy_agent_env(std::move(legacy_agent_env))
        , m_ingestion_status_ok2(std::move(ingestion_status_ok2))
    {}

    array<multiItem> m_metrics;
    array<sampleFactor> m_sample_factors;
    array<sampleFactor> m_ingestion_status_ok;
    Nat m_missed_seconds;
    Int m_legacy_agent_env;
    array<ingestionStatus2> m_ingestion_status_ok2;
};

bool operator==(const SourceBucket2& lhs, const SourceBucket2& rhs) noexcept
{
    return lhs.get_metrics() == rhs.get_metrics()
           && lhs.get_sample_factors() == rhs.get_sample_factors()
           && lhs.get_ingestion_status_ok() == rhs.get_ingestion_status_ok()
           && lhs.get_missed_seconds() == rhs.get_missed_seconds()
           && lhs.get_legacy_agent_env() == rhs.get_legacy_agent_env()
           && lhs.get_ingestion_status_ok2() == rhs.get_ingestion_status_ok2();
}

size_t consume(const SourceBucket2& value) noexcept
{
    size_t result = 0;
    result += consume(value.get_metrics());
    result += consume(value.get_sample_factors());
    result += consume(value.get_ingestion_status_ok());
    result += consume(value.get_missed_seconds());
    result += consume(value.get_legacy_agent_env());
    result += consume(value.get_ingestion_status_ok2());
    return result;
}

}    // namespace opus::proxy::statshouse
