#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"



namespace fuzz_details {

struct IngestionStatus2 {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::statshouse::IngestionStatus2_BASE<BOXED>::Builder {}
                .set_env(env)
                .set_metric(metric)
                .set_value(value);
    }

    int env;
    int metric;
    float value;
};

auto domainIngestionStatus2()
{
    return fuzztest::StructOf<IngestionStatus2>(fuzztest::Arbitrary<int>(),
                                                fuzztest::Arbitrary<int>(),
                                                fuzztest::Finite<float>());
}



template <bool INNER_BOXED>
struct ArrayIngestionStatus2 {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::IngestionStatus2_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::IngestionStatus2_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<IngestionStatus2> arr;
};

template <bool INNER_BOXED>
auto domainArrayIngestionStatus2()
{
    return fuzztest::StructOf<ArrayIngestionStatus2<INNER_BOXED>>(fuzztest::VectorOf(domainIngestionStatus2()).WithMaxSize(1024));
}

}    // namespace fuzz_details
