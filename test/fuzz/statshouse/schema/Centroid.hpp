#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"



namespace fuzz_details {

struct Centroid {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::statshouse::Centroid_BASE<BOXED>::Builder {}
                .set_value(value)
                .set_weight(weight);
    }

    float value;
    float weight;
};

auto domainCentroid()
{
    return fuzztest::StructOf<Centroid>(fuzztest::Finite<float>(),
                                        fuzztest::Finite<float>());
}



template <bool INNER_BOXED>
struct ArrayCentroid {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::Centroid_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::Centroid_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<Centroid> arr;
};

template <bool INNER_BOXED>
auto domainArrayCentroid()
{
    return fuzztest::StructOf<ArrayCentroid<INNER_BOXED>>(fuzztest::VectorOf(domainCentroid()).WithMaxSize(1024));
}

}    // namespace fuzz_details
