#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/simple/Point.hpp"



namespace fuzz_details {

struct Rectangle {
    static constexpr Nat MAX_MASK_VALUE = 7;

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename simple::Rectangle_BASE<BOXED>::Builder {}
                .set_a(a.toBuilder<false>())
                .set_b(b.toBuilder<false>());
    }

    Point a;
    Point b;
};

auto domainRectangle()
{
    return fuzztest::StructOf<Rectangle>(domainPoint(),
                                         domainPoint());
}



template <bool INNER_BOXED>
struct ArrayRectangle {
    static constexpr Nat MAX_MASK_VALUE = 7;

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename simple::Rectangle_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<simple::Rectangle_BASE<INNER_BOXED>, BOXED>::Builder {}
                .setArray(tmp);
    }

    std::vector<Rectangle> arr;
};

template <bool INNER_BOXED>
auto domainArrayRectangle()
{
    return fuzztest::StructOf<ArrayRectangle<INNER_BOXED>>(fuzztest::VectorOf(domainRectangle()).WithMaxSize(128));
}



template <Nat MASK>
struct Rectangle_s {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename simple::Rectangle_BASE<BOXED, MASK>::Builder {}
                .set_a(a.template toBuilder<false>())
                .set_b(b.template toBuilder<false>());
    }

    Point_s<MASK> a;
    Point_s<MASK> b;
};

template <Nat MASK>
auto domainRectangle_s()
{
    return fuzztest::StructOf<Rectangle_s<MASK>>(domainPoint_s<MASK>(),
                                                 domainPoint_s<MASK>());
}



template <bool INNER_BOXED, Nat MASK>
struct ArrayRectangle_s {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename simple::Rectangle_BASE<INNER_BOXED, MASK>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<simple::Rectangle_BASE<INNER_BOXED, MASK>, BOXED>::Builder {}
                .setArray(tmp);
    }

    std::vector<Rectangle_s<MASK>> arr;
};

template <bool INNER_BOXED, Nat MASK>
auto domainArrayRectangle_s()
{
    return fuzztest::StructOf<ArrayRectangle_s<INNER_BOXED, MASK>>(fuzztest::VectorOf(domainRectangle_s<MASK>()).WithMaxSize(128));
}

}    // namespace fuzz_details
