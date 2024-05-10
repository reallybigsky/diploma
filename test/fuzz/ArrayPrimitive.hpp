#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"



namespace fuzz_details {

template <::Primitive T>
struct ArrayPrimitive {
    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::ArrayBase<BOXED, ::Scalar<T>>::Builder {}
                .setArray(arr);
    }

    std::vector<T> arr;
};

template <::Primitive T>
auto domainArrayPrimitive()
{
    if constexpr (std::is_integral_v<T>) {
        return fuzztest::StructOf<ArrayPrimitive<T>>(fuzztest::Arbitrary<std::vector<T>>().WithMaxSize(1024));
    } else {
        return fuzztest::StructOf<ArrayPrimitive<T>>(fuzztest::VectorOf(fuzztest::Finite<T>()).WithMaxSize(1024));
    }
}

template <::Primitive T, bool INNER_BOXED>
struct ArrayArrayPrimitive {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::ArrayBase<INNER_BOXED, Scalar<T>>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::ArrayBase<INNER_BOXED, Scalar<T>>>::Builder {}
                .setArray(tmp);
    }

    std::vector<ArrayPrimitive<T>> arr;
};

template <::Primitive T, bool INNER_BOXED>
auto domainArrayArrayPrimitive()
{
    return fuzztest::StructOf<ArrayArrayPrimitive<T, INNER_BOXED>>(fuzztest::VectorOf(domainArrayPrimitive<T>()).WithMaxSize(128));
}

}    // namespace fuzz_details
