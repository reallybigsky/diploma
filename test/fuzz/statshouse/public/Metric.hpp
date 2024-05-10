#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/ArrayPrimitive.hpp"
#include "test/fuzz/String.hpp"
#include "test/fuzz/statshouse/Dictionary.hpp"



namespace fuzz_details {

struct Metric {
    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::statshouse::Metric_BASE<BOXED>::Builder {}
                .set_fields_mask(fields_mask)
                .set_name(name.toBuilder<false>())
                .set_tags(tags.toBuilder<false>())
                .set_counter(counter)
                .set_ts(ts)
                .set_value(value.toBuilder<false>())
                .set_unique(unique.toBuilder<false>());
    }

    uint32_t fields_mask;
    String name;
    Dictionary_String tags;
    double counter;
    uint32_t ts;
    ArrayPrimitive<double> value;
    ArrayPrimitive<long> unique;
};

auto domainMetric()
{
    return fuzztest::StructOf<Metric>(fuzztest::InRange<uint32_t>(0, 31),
                                      domainString(),
                                      domainDictionary_String(),
                                      fuzztest::Finite<double>(),
                                      fuzztest::Arbitrary<uint32_t>(),
                                      domainArrayPrimitive<double>(),
                                      domainArrayPrimitive<long>());
}



template <bool INNER_BOXED>
struct ArrayMetric {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::Metric_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::Metric_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<Metric> arr;
};

template <bool INNER_BOXED>
auto domainArrayMetric()
{
    return fuzztest::StructOf<ArrayMetric<INNER_BOXED>>(fuzztest::VectorOf(domainMetric()).WithMaxSize(64));
}

}    // namespace fuzz_details
