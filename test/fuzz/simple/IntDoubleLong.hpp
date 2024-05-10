#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"



namespace fuzz_details {

struct IntDoubleLong {
    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename simple::IntDoubleLong_BASE<BOXED>::Builder {}
                .set_Int1(Int1)
                .set_Double1(Double1)
                .set_Long1(Long1);
    }

    int Int1;
    double Double1;
    long Long1;
};

auto domainIntDoubleLong()
{
    return fuzztest::StructOf<IntDoubleLong>(fuzztest::Arbitrary<int>(),
                                             fuzztest::Finite<double>(),
                                             fuzztest::Arbitrary<long>());
}



template <bool INNER_BOXED>
struct ArrayIntDoubleLong {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename simple::IntDoubleLong_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, simple::IntDoubleLong_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<IntDoubleLong> arr;
};

template <bool INNER_BOXED>
auto domainArrayIntDoubleLong()
{
    return fuzztest::StructOf<ArrayIntDoubleLong<INNER_BOXED>>(fuzztest::VectorOf(domainIntDoubleLong()).WithMaxSize(1024));
}

}    // namespace fuzz_details