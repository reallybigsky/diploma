#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"



namespace fuzz_details {

struct Flag {

    template <bool = true>
    auto toBuilder() const noexcept
    {
        return ::statshouse::Flag::Builder::random(DEFAULT_RANDOM_ENGINE);
    }
};

auto domainFlag()
{
    return fuzztest::StructOf<Flag>();
}



struct ArrayFlag {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::Flag::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<true>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::Flag>::Builder {}
                .setArray(tmp);
    }

    std::vector<Flag> arr;
};

auto domainArrayFlag()
{
    return fuzztest::StructOf<ArrayFlag>(fuzztest::VectorOf(domainFlag()).WithMaxSize(1024));
}

}    // namespace fuzz_details
