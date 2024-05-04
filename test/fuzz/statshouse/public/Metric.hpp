#pragma once

#include "fuzztest/fuzztest.h"

#include "Environment.hpp"

#include "fuzz/ArrayPrimitive.hpp"
#include "fuzz/statshouse/Dictionary.hpp"

#include "baseline/statshouse/public/Metric.hpp"



namespace fuzz_details {

struct Metric {
    using Type = ::statshouse::Metric;
    using Builder = Type::Builder;

    Builder toBuilder() const noexcept
    {
        Builder result;
        result.set_fields_mask(s_fields_mask)
                .set_name(s_name.toBuilder())
                .set_tags(s_tags.toBuilder())
                .set_counter(s_counter)
                .set_ts(s_ts)
                .set_value(s_value.toBuilder())
                .set_unique(s_unique.toBuilder());

        return result;
    }

    uint32_t s_fields_mask;
    string s_name;
    DictionaryString s_tags;
    double s_counter;
    uint32_t s_ts;
    arrayDouble s_value;
    arrayLong s_unique;
};

auto domainMetric()
{
    return fuzztest::StructOf<Metric>(fuzztest::InRange<uint32_t>(0, 15),
                                      domainString(),
                                      domainDictionaryString(),
                                      fuzztest::Finite<double>(),
                                      fuzztest::Arbitrary<uint32_t>(),
                                      domainArrayDouble(),
                                      domainArrayLong());
}

}    // namespace fuzz_details
