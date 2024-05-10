#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/String.hpp"
#include "test/fuzz/statshouse/schema/MultiValue.hpp"



namespace fuzz_details {

struct TopElement {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::statshouse::TopElement_BASE<BOXED>::Builder {}
                .set_key(key.toBuilder<false>())
                .set_fields_mask(fields_mask)
                .set_value(value.toBuilder<false>());
    }

    String key;
    uint32_t fields_mask;
    MultiValue value;
};

auto domainTopElement()
{
    return fuzztest::StructOf<TopElement>(domainString(),
                                          fuzztest::InRange<uint32_t>(0, 511),
                                          domainMultiValue());
}



template <bool INNER_BOXED>
struct ArrayTopElement {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::TopElement_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::TopElement_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<TopElement> arr;
};

template <bool INNER_BOXED>
auto domainArrayTopElement()
{
    return fuzztest::StructOf<ArrayTopElement<INNER_BOXED>>(fuzztest::VectorOf(domainTopElement()).WithMaxSize(32));
}

}    // namespace fuzz_details
