#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/simple/IntDouble.hpp"
#include "test/fuzz/simple/IntDoubleLong.hpp"



namespace fuzz_details {

struct CompoundCTS {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename simple::CompoundCTS_BASE<BOXED>::Builder {}
                .set_IntDouble1(IntDouble1.toBuilder<true>())
                .set_Long1(Long1)
                .set_IntDoubleLong1(IntDoubleLong1.toBuilder<false>());
    }

    IntDouble IntDouble1;
    long Long1;
    IntDoubleLong IntDoubleLong1;
};

auto domainCompoundCTS()
{
    return fuzztest::StructOf<CompoundCTS>(domainIntDouble(),
                                           fuzztest::Arbitrary<long>(),
                                           domainIntDoubleLong());
}


template <bool INNER_BOXED>
struct ArrayCompoundCTS {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename simple::CompoundCTS_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<simple::CompoundCTS_BASE<INNER_BOXED>, BOXED>::Builder {}
                .setArray(tmp);
    }

    std::vector<CompoundCTS> arr;
};

template <bool INNER_BOXED>
auto domainArrayCompoundCTS()
{
    return fuzztest::StructOf<ArrayCompoundCTS<INNER_BOXED>>(fuzztest::VectorOf(domainCompoundCTS()).WithMaxSize(1024));
}

}    // namespace fuzz_details
