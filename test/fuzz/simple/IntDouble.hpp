#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"



namespace fuzz_details {

struct IntDouble {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename simple::IntDouble_BASE<BOXED>::Builder {}
                .set_Int1(Int1)
                .set_Double1(Double1);
    }

    int Int1;
    double Double1;
};

auto domainIntDouble()
{
    return fuzztest::StructOf<IntDouble>(fuzztest::Arbitrary<int>(),
                                         fuzztest::Finite<double>());
}



template <bool INNER_BOXED>
struct ArrayIntDouble {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename simple::IntDouble_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<simple::IntDouble_BASE<INNER_BOXED>, BOXED>::Builder {}
                .setArray(tmp);
    }

    std::vector<IntDouble> arr;
};

template <bool INNER_BOXED>
auto domainArrayIntDouble()
{
    return fuzztest::StructOf<ArrayIntDouble<INNER_BOXED>>(fuzztest::VectorOf(domainIntDouble()).WithMaxSize(1024));
}

}    // namespace fuzz_details
