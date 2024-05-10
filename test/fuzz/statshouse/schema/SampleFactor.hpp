#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"



namespace fuzz_details {

struct SampleFactor {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::statshouse::SampleFactor_BASE<BOXED>::Builder {}
                .set_metric(metric)
                .set_value(value);
    }

    int metric;
    float value;
};

auto domainSampleFactor()
{
    return fuzztest::StructOf<SampleFactor>(fuzztest::Arbitrary<int>(),
                                            fuzztest::Finite<float>());
}



template <bool INNER_BOXED>
struct ArraySampleFactor {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::SampleFactor_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::SampleFactor_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<SampleFactor> arr;
};

template <bool INNER_BOXED>
auto domainArraySampleFactor()
{
    return fuzztest::StructOf<ArraySampleFactor<INNER_BOXED>>(fuzztest::VectorOf(domainSampleFactor()).WithMaxSize(1024));
}

}    // namespace fuzz_details
