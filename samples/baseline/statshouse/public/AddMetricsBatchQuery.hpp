#pragma once

#include "prelude/baseline/Includes.hpp"

#include "samples/baseline/statshouse/public/Metric.hpp"



namespace baseline::statshouse {

class AddMetricsBatchQuery {
public:
    static constexpr Magic MAGIC = 2586840297;

    AddMetricsBatchQuery() noexcept = default;

    AddMetricsBatchQuery(const AddMetricsBatchQuery&) noexcept = default;
    AddMetricsBatchQuery(AddMetricsBatchQuery&&) noexcept = default;
    AddMetricsBatchQuery& operator=(const AddMetricsBatchQuery&) noexcept = default;
    AddMetricsBatchQuery& operator=(AddMetricsBatchQuery&&) noexcept = default;

    ~AddMetricsBatchQuery() noexcept = default;

    const Nat& get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const array<metric>& get_metrics() const noexcept
    {
        return m_metrics;
    }

    static AddMetricsBatchQuery fetch(InputStream& stream)
    {
        Nat fields_mask = Nat::fetch(stream);
        array<metric> metrics = array<metric>::fetch(stream);
        AddMetricsBatchQuery result {std::move(fields_mask),
                                     std::move(metrics)};
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_fields_mask.store(stream);
        m_metrics.store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const AddMetricsBatchQuery& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_metrics == rhs.get_metrics();
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3, size_t SIZE_4, size_t SIZE_5, size_t SIZE_6, size_t SIZE_7>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(Nat::Builder::random(engine))
                    .set_metrics(array<metric>::Builder::random<SIZE_1, SIZE_2, SIZE_3, SIZE_4, SIZE_5, SIZE_6, SIZE_7>(engine));
        }

        Builder& set_fields_mask(const Nat::Builder& value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_metrics(const array<metric>::Builder& value) noexcept
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
        array<metric>::Builder b_metrics;
    };

private:
    AddMetricsBatchQuery(Nat&& fields_mask,
                         array<metric>&& metrics) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_metrics(std::move(metrics))
    {}

    Nat m_fields_mask;
    array<metric> m_metrics;
};

bool operator==(const AddMetricsBatchQuery& lhs, const AddMetricsBatchQuery& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_metrics() == rhs.get_metrics();
}

size_t consume(const AddMetricsBatchQuery& value) noexcept
{
    size_t result = 0;
    result += consume(value.get_fields_mask());
    result += consume(value.get_metrics());
    return result;
}

}    // namespace baseline::statshouse
