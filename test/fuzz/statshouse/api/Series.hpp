#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/ArrayPrimitive.hpp"



namespace fuzz_details {

struct Series {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::statshouse::Series_BASE<BOXED>::Builder {}
                .set_fields_mask(fields_mask)
                .set_series_data(series_data.toBuilder<false>())
                .set_time(time.toBuilder<false>());
    }

    uint32_t fields_mask;
    ArrayArrayPrimitive<double, false> series_data;
    ArrayPrimitive<long> time;
};

auto domainSeries()
{
    return fuzztest::StructOf<Series>(fuzztest::Arbitrary<uint32_t>(),
                                      domainArrayArrayPrimitive<double, false>(),
                                      domainArrayPrimitive<long>());
}



template <bool INNER_BOXED>
struct ArraySeries {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::Series_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::Series_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<Series> arr;
};

template <bool INNER_BOXED>
auto domainArraySeries()
{
    return fuzztest::StructOf<ArraySeries<INNER_BOXED>>(fuzztest::VectorOf(domainSeries()).WithMaxSize(64));
}

}    // namespace fuzz_details
