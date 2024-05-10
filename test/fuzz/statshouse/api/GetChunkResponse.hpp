#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/statshouse/api/Series.hpp"



namespace fuzz_details {

struct GetChunkResponse {

    auto toBuilder() const noexcept
    {
        return typename ::statshouse::GetChunkResponse::Builder {}
                .set_fields_mask(fields_mask)
                .set_series(series.toBuilder<false>())
                .set_index(index);
    }

    uint32_t fields_mask;
    Series series;
    int index;
};

auto domainGetChunkResponse()
{
    return fuzztest::StructOf<GetChunkResponse>(fuzztest::Arbitrary<uint32_t>(),
                                                domainSeries(),
                                                fuzztest::Arbitrary<int>());
}

}    // namespace fuzz_details
