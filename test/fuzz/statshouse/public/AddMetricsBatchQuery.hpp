#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/statshouse/public/Metric.hpp"



namespace fuzz_details {

struct AddMetricsBatchQuery {

    auto toBuilder() const noexcept
    {
        return typename ::statshouse::AddMetricsBatchQuery::Builder {}
                .set_fields_mask(fields_mask)
                .set_metrics(metrics.toBuilder<false>());
    }

    uint32_t fields_mask;
    ArrayMetric<false> metrics;
};

auto domainAddMetricsBatchQuery()
{
    return fuzztest::StructOf<AddMetricsBatchQuery>(fuzztest::Arbitrary<uint32_t>(),
                                                    domainArrayMetric<false>());
}

}    // namespace fuzz_details
