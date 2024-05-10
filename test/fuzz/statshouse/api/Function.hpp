#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"



namespace fuzz_details {

struct Function {

    template <bool = true>
    auto toBuilder() const noexcept
    {
        return ::statshouse::Function::Builder::random(DEFAULT_RANDOM_ENGINE);
    }
};

auto domainFunction()
{
    return fuzztest::StructOf<Function>();
}



struct ArrayFunction {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::Function::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<true>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::Function>::Builder {}
                .setArray(tmp);
    }

    std::vector<Function> arr;
};

auto domainArrayFunction()
{
    return fuzztest::StructOf<ArrayFunction>(fuzztest::VectorOf(domainFunction()).WithMaxSize(1024));
}

}    // namespace fuzz_details
