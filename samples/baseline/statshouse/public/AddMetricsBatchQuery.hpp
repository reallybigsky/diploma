#pragma once

#include "baseline/Includes.hpp"
#include "baseline/statshouse/public/Metric.hpp"



namespace baseline::statshouse {

class AddMetricsBatchQuery {
public:
    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
    static constexpr Magic MAGIC = 2586840297;
    static constexpr bool CTS = false;

    AddMetricsBatchQuery() noexcept = default;

    AddMetricsBatchQuery(const AddMetricsBatchQuery&) noexcept = default;
    AddMetricsBatchQuery(AddMetricsBatchQuery&&) noexcept = default;
    AddMetricsBatchQuery& operator=(const AddMetricsBatchQuery&) noexcept = default;
    AddMetricsBatchQuery& operator=(AddMetricsBatchQuery&&) noexcept = default;

    ~AddMetricsBatchQuery() noexcept = default;

    AddMetricsBatchQuery(Nat fields_mask,
                         const array<Metric>& metrics) noexcept
        : m_fields_mask(fields_mask)
        , m_metrics(metrics)
    {}

    Nat get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const array<Metric>& get_metrics() const noexcept
    {
        return m_metrics;
    }

    static AddMetricsBatchQuery fetch(InputStream& stream)
    {
        // HighLevel => fetch magic in higher variant
        //
        // Magic magic = Magic::fetch(stream);
        // if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);

        Nat fields_mask = Nat::fetch(stream);
        array<Metric> metrics = array<Metric>::fetch(stream);
        return {fields_mask,
                metrics};
    }

    void store(OutputStream& stream) const
    {
        MAGIC.store(stream);
        m_fields_mask.store(stream);
        m_metrics.store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const AddMetricsBatchQuery& rhs)
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_metrics == rhs.get_metrics();
        }

        Builder& set_fields_mask(Nat::Builder value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_metrics(const array<Metric>::Builder& value) noexcept
        {
            b_metrics = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            MAGIC.store(stream);
            b_fields_mask.store(stream);
            b_metrics.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        array<Metric>::Builder b_metrics;
    };

private:
    Nat m_fields_mask;
    array<Metric> m_metrics;
};

}    // namespace baseline::statshouse
