#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/String.hpp"
#include "test/fuzz/statshouse/api/Flag.hpp"



namespace fuzz_details {

struct TagValue {
    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::statshouse::TagValue_BASE<BOXED>::Builder {}
                .set_fields_mask(fields_mask)
                .set_in(in)
                .set_value(value.toBuilder<false>())
                .set_flag(flag.toBuilder<>());
    }

    uint32_t fields_mask;
    int in;
    String value;
    Flag flag;
};

auto domainTagValue()
{
    return fuzztest::StructOf<TagValue>(fuzztest::Arbitrary<uint32_t>(),
                                        fuzztest::Arbitrary<int>(),
                                        domainString(),
                                        domainFlag());
}


template <bool INNER_BOXED>
struct ArrayTagValue {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::TagValue_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::TagValue_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<TagValue> arr;
};

template <bool INNER_BOXED>
auto domainArrayTagValue()
{
    return fuzztest::StructOf<ArrayTagValue<INNER_BOXED>>(fuzztest::VectorOf(domainTagValue()).WithMaxSize(64));
}

}    // namespace fuzz_details
