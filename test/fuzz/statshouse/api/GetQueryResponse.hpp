#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/ArrayPrimitive.hpp"
#include "test/fuzz/statshouse/api/Series.hpp"
#include "test/fuzz/statshouse/api/SeriesMeta.hpp"



namespace fuzz_details {

struct GetQueryResponse {

    auto toBuilder() const noexcept
    {
        return typename ::statshouse::GetQueryResponse::Builder {}
                .set_query_fields_mask(query_fields_mask)
                .set_fields_mask(fields_mask)
                .set_series(series.toBuilder<false>())
                .set_series_meta(series_meta.toBuilder<false>())
                .set_chunk_ids(chunk_ids.toBuilder<false>())
                .set_total_time_points(total_time_points)
                .set_response_id(response_id);
    }

    uint32_t query_fields_mask;
    uint32_t fields_mask;
    Series series;
    ArraySeriesMeta<false> series_meta;
    ArrayPrimitive<int> chunk_ids;
    int total_time_points;
    long response_id;
};

auto domainGetQueryResponse()
{
    return fuzztest::StructOf<GetQueryResponse>(fuzztest::InRange<uint32_t>(16, 511),
                                                fuzztest::InRange<uint32_t>(0, 3),
                                                domainSeries(),
                                                domainArraySeriesMeta<false>(),
                                                domainArrayPrimitive<int>(),
                                                fuzztest::Arbitrary<int>(),
                                                fuzztest::Arbitrary<long>());
}

}    // namespace fuzz_details
