#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"



namespace fuzz_details {

struct String {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::ArrayBase<BOXED, symbol_t>::Builder {}
                .setString(str);
    }

    std::string str;
};

auto domainSmallString()
{
    return fuzztest::StructOf<String>(fuzztest::String().WithMinSize(0).WithMaxSize(254));
}

auto domainBigString()
{
    return fuzztest::StructOf<String>(fuzztest::String().WithMinSize(255).WithMaxSize(1024));
}

auto domainString()
{
    return fuzztest::OneOf(domainSmallString(), domainBigString());
}



template <bool INNER_BOXED>
struct ArrayString {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::ArrayBase<INNER_BOXED, symbol_t>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::ArrayBase<INNER_BOXED, symbol_t>>::Builder {}
                .setArray(tmp);
    }

    std::vector<String> arr;
};

template <bool INNER_BOXED>
auto domainArrayString()
{
    return fuzztest::StructOf<ArrayString<INNER_BOXED>>(fuzztest::VectorOf(domainString()).WithMaxSize(1024));
}

}    // namespace fuzz_details
