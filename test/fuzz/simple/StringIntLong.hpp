#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/String.hpp"



namespace fuzz_details {

struct StringIntLong {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename simple::StringIntLong_BASE<BOXED>::Builder {}
                .set_String1(String1.toBuilder<true>())
                .set_Int1(Int1)
                .set_Long1(Long1);
    }


    String String1;
    int Int1;
    long Long1;
};

auto domainStringIntLong()
{
    return fuzztest::StructOf<StringIntLong>(domainString(),
                                             fuzztest::Arbitrary<int>(),
                                             fuzztest::Arbitrary<long>());
}



template <bool INNER_BOXED>
struct ArrayStringIntLong {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename simple::StringIntLong_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<simple::StringIntLong_BASE<INNER_BOXED>, BOXED>::Builder {}
                .setArray(tmp);
    }

    std::vector<StringIntLong> arr;
};

template <bool INNER_BOXED>
auto domainArrayStringIntLong()
{
    return fuzztest::StructOf<ArrayStringIntLong<INNER_BOXED>>(fuzztest::VectorOf(domainStringIntLong()).WithMaxSize(128));
}

}    // namespace fuzz_details
