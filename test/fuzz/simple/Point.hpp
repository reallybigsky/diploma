#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"



namespace fuzz_details {

struct Point {
    static constexpr Nat MIN_MASK_VALUE = 0;
    static constexpr Nat MAX_MASK_VALUE = 7;

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename simple::Point_BASE<BOXED>::Builder {}
                .set_x(x)
                .set_y(y)
                .set_z(z);
    }

    long x;
    long y;
    long z;
};

auto domainPoint()
{
    return fuzztest::StructOf<Point>(fuzztest::Arbitrary<long>(),
                                     fuzztest::Arbitrary<long>(),
                                     fuzztest::Arbitrary<long>());
}



template <bool INNER_BOXED>
struct ArrayPoint {
    static constexpr Nat MIN_MASK_VALUE = 0;
    static constexpr Nat MAX_MASK_VALUE = 7;

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename simple::Point_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, simple::Point_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<Point> arr;
};

template <bool INNER_BOXED>
auto domainArrayPoint()
{
    return fuzztest::StructOf<ArrayPoint<INNER_BOXED>>(fuzztest::VectorOf(domainPoint()).WithMaxSize(1024));
}



template <Nat MASK>
struct Point_s {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        typename simple::Point_BASE<BOXED, MASK>::Builder result;

        if constexpr (IS_SET(MASK, 0))
            result.set_x(x);

        if constexpr (IS_SET(MASK, 1))
            result.set_y(y);

        if constexpr (IS_SET(MASK, 2))
            result.set_z(z);

        return result;
    }

    long x;
    long y;
    long z;
};

template <Nat MASK>
auto domainPoint_s()
{
    return fuzztest::StructOf<Point_s<MASK>>(fuzztest::Arbitrary<long>(),
                                             fuzztest::Arbitrary<long>(),
                                             fuzztest::Arbitrary<long>());
}



template <bool INNER_BOXED, Nat MASK>
struct ArrayPoint_s {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename simple::Point_BASE<INNER_BOXED, MASK>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, simple::Point_BASE<INNER_BOXED, MASK>>::Builder {}
                .setArray(tmp);
    }

    std::vector<Point_s<MASK>> arr;
};

template <bool INNER_BOXED, Nat MASK>
auto domainArrayPoint_s()
{
    return fuzztest::StructOf<ArrayPoint_s<INNER_BOXED, MASK>>(fuzztest::VectorOf(domainPoint_s<MASK>()).WithMaxSize(1024));
}

}    // namespace fuzz_details
