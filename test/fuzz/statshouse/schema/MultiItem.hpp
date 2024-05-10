#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/ArrayPrimitive.hpp"
#include "test/fuzz/statshouse/schema/MultiValue.hpp"
#include "test/fuzz/statshouse/schema/TopElement.hpp"



namespace fuzz_details {

struct MultiItem {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::statshouse::MultiItem_BASE<BOXED>::Builder {}
                .set_fields_mask(fields_mask)
                .set_metric(metric)
                .set_keys(keys.toBuilder<false>())
                .set_t(t)
                .set_tail(tail.toBuilder<false>())
                .set_top(top.toBuilder<false>());
    }

    uint32_t fields_mask;
    int metric;
    ArrayPrimitive<int> keys;
    uint32_t t;
    MultiValue tail;
    ArrayTopElement<false> top;
};

auto domainMultiItem()
{
    return fuzztest::StructOf<MultiItem>(fuzztest::InRange<uint32_t>(0, 4095),
                                         fuzztest::Arbitrary<int>(),
                                         domainArrayPrimitive<int>(),
                                         fuzztest::Arbitrary<uint32_t>(),
                                         domainMultiValue(),
                                         domainArrayTopElement<false>());
}


template <bool INNER_BOXED>
struct ArrayMultiItem {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::MultiItem_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::MultiItem_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<MultiItem> arr;
};

template <bool INNER_BOXED>
auto domainArrayMultiItem()
{
    return fuzztest::StructOf<ArrayMultiItem<INNER_BOXED>>(fuzztest::VectorOf(domainMultiItem()).WithMaxSize(16));
}

}    // namespace fuzz_details
