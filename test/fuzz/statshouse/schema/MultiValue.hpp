#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/String.hpp"
#include "test/fuzz/statshouse/schema/Centroid.hpp"



namespace fuzz_details {

struct MultiValue {
    static constexpr Nat MIN_MASK_VALUE = 0;
    static constexpr Nat MAX_MASK_VALUE = 511;

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::statshouse::MultiValue_BASE<BOXED>::Builder {}
                .set_counter(counter)
                .set_value_min(value_min)
                .set_value_max(value_max)
                .set_value_sum(value_sum)
                .set_value_sum_square(value_sum_square)
                .set_uniques(uniques.toBuilder<false>())
                .set_centroids(centroids.toBuilder<false>())
                .set_max_host_tag(max_host_tag)
                .set_min_host_tag(min_host_tag)
                .set_max_counter_host_tag(max_counter_host_tag);
    }

    double counter;
    double value_min;
    double value_max;
    double value_sum;
    double value_sum_square;
    String uniques;
    ArrayCentroid<false> centroids;
    int max_host_tag;
    int min_host_tag;
    int max_counter_host_tag;
};

auto domainMultiValue()
{
    return fuzztest::StructOf<MultiValue>(fuzztest::Finite<double>(),
                                          fuzztest::Finite<double>(),
                                          fuzztest::Finite<double>(),
                                          fuzztest::Finite<double>(),
                                          fuzztest::Finite<double>(),
                                          domainString(),
                                          domainArrayCentroid<false>(),
                                          fuzztest::Arbitrary<int>(),
                                          fuzztest::Arbitrary<int>(),
                                          fuzztest::Arbitrary<int>());
}



template <bool INNER_BOXED>
struct ArrayMultiValue {
    static constexpr Nat MIN_MASK_VALUE = 0;
    static constexpr Nat MAX_MASK_VALUE = 511;

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::MultiValue_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::MultiValue_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<MultiValue> arr;
};

template <bool INNER_BOXED>
auto domainArrayMultiValue()
{
    return fuzztest::StructOf<ArrayMultiValue<INNER_BOXED>>(fuzztest::VectorOf(domainMultiValue()).WithMaxSize(256));
}

}    // namespace fuzz_details
