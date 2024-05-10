#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/statshouse/schema/IngestionStatus2.hpp"
#include "test/fuzz/statshouse/schema/MultiItem.hpp"
#include "test/fuzz/statshouse/schema/SampleFactor.hpp"



namespace fuzz_details {

struct SourceBucket2 {

    auto toBuilder() const noexcept
    {
        return typename ::statshouse::SourceBucket2::Builder {}
                .set_metrics(metrics.toBuilder<false>())
                .set_sample_factors(sample_factors.toBuilder<false>())
                .set_ingestion_status_ok(ingestion_status_ok.toBuilder<false>())
                .set_missed_seconds(missed_seconds)
                .set_legacy_agent_env(legacy_agent_env)
                .set_ingestion_status_ok2(ingestion_status_ok2.toBuilder<false>());
    }

    ArrayMultiItem<false> metrics;
    ArraySampleFactor<false> sample_factors;
    ArraySampleFactor<false> ingestion_status_ok;
    uint32_t missed_seconds;
    int legacy_agent_env;
    ArrayIngestionStatus2<false> ingestion_status_ok2;
};

auto domainSourceBucket2()
{
    return fuzztest::StructOf<SourceBucket2>(domainArrayMultiItem<false>(),
                                             domainArraySampleFactor<false>(),
                                             domainArraySampleFactor<false>(),
                                             fuzztest::Arbitrary<uint32_t>(),
                                             fuzztest::Arbitrary<int>(),
                                             domainArrayIngestionStatus2<false>());
}

}    // namespace fuzz_details
