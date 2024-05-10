#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/String.hpp"
#include "test/fuzz/ArrayPrimitive.hpp"
#include "test/fuzz/statshouse/api/Filter.hpp"
#include "test/fuzz/statshouse/api/Function.hpp"



namespace fuzz_details {

struct Query {

    auto toBuilder() const noexcept
    {
        return typename ::statshouse::Query::Builder {}
                .set_fields_mask(fields_mask)
                .set_version(version)
                .set_top_n(top_n)
                .set_metric_name(metric_name.toBuilder<false>())
                .set_time_from(time_from)
                .set_time_to(time_to)
                .set_interval(interval.toBuilder<false>())
                .set_function(function.toBuilder<>())
                .set_group_by(group_by.toBuilder<false>())
                .set_filters(filters.toBuilder<false>())
                .set_time_shift(time_shift.toBuilder<false>())
                .set_promql(promql.toBuilder<false>())
                .set_what(what.toBuilder<false>())
                .set_widthAgg(widthAgg.toBuilder<false>());
    }

    uint32_t fields_mask;
    int version;
    int top_n;
    String metric_name;
    long time_from;
    long time_to;
    String interval;
    Function function;
    ArrayString<false> group_by;
    ArrayFilter<false> filters;
    ArrayPrimitive<long> time_shift;
    String promql;
    ArrayFunction what;
    String widthAgg;
};

auto domainQuery()
{
    return fuzztest::StructOf<Query>(fuzztest::InRange<uint32_t>(0, 255),
                                     fuzztest::Arbitrary<int>(),
                                     fuzztest::Arbitrary<int>(),
                                     domainString(),
                                     fuzztest::Arbitrary<long>(),
                                     fuzztest::Arbitrary<long>(),
                                     domainString(),
                                     domainFunction(),
                                     domainArrayString<false>(),
                                     domainArrayFilter<false>(),
                                     domainArrayPrimitive<long>(),
                                     domainString(),
                                     domainArrayFunction(),
                                     domainString());
}

}    // namespace fuzz_details
