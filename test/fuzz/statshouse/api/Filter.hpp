#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/String.hpp"
#include "test/fuzz/statshouse/api/TagValue.hpp"



namespace fuzz_details {

struct Filter {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::statshouse::Filter_BASE<BOXED>::Builder {}
                .set_fields_mask(fields_mask)
                .set_key(key.toBuilder<false>())
                .set_values(values.toBuilder<false>());
    }

    uint32_t fields_mask;
    String key;
    ArrayTagValue<false> values;
};

auto domainFilter()
{
    return fuzztest::StructOf<Filter>(fuzztest::Arbitrary<uint32_t>(),
                                      domainString(),
                                      domainArrayTagValue<false>());
}



template <bool INNER_BOXED>
struct ArrayFilter {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::Filter_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::Filter_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<Filter> arr;
};

template <bool INNER_BOXED>
auto domainArrayFilter()
{
    return fuzztest::StructOf<ArrayFilter<INNER_BOXED>>(fuzztest::VectorOf(domainFilter()).WithMaxSize(64));
}

}    // namespace fuzz_details
