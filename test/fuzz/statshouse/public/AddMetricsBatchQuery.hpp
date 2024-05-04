#pragma once

#include "fuzztest/fuzztest.h"

#include "Environment.hpp"

#include "fuzz/statshouse/public/Metric.hpp"

#include "baseline/statshouse/public/AddMetricsBatchQuery.hpp"



namespace fuzz_details {

struct AddMetricsBatchQuery {
    using Type = ::statshouse::AddMetricsBatchQuery;
    using Builder = Type::Builder;

    Builder toBuilder() const noexcept
    {
        Builder result;

        std::vector<::statshouse::Metric::Builder> arr;
        arr.reserve(s_metrics.size());
        for (const auto& value : s_metrics) {
            arr.emplace_back(value.toBuilder());
        }

        result.set_fields_mask(s_fields_mask)
                .set_metrics(arr);

        return result;
    }

    uint32_t s_fields_mask;
    std::vector<Metric> s_metrics;
};

auto domainAddMetricsBatchQuery()
{
    return fuzztest::StructOf<AddMetricsBatchQuery>(fuzztest::Arbitrary<uint32_t>(),
                                                    fuzztest::VectorOf(domainMetric()).WithMaxSize(1000));
}

}    // namespace fuzz_details
