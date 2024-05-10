#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/String.hpp"
#include "test/fuzz/statshouse/Dictionary.hpp"
#include "test/fuzz/statshouse/api/Function.hpp"



namespace fuzz_details {

struct SeriesMeta {
    static constexpr Nat MIN_MASK_VALUE = 0;
    static constexpr Nat MAX_MASK_VALUE = 511;

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::statshouse::SeriesMeta_BASE<BOXED>::Builder {}
                .set_fields_mask(fields_mask)
                .set_time_shift(time_shift)
                .set_tags(tags.toBuilder<false>())
                .set_what(what.toBuilder<>())
                .set_name(name.toBuilder<false>())
                .set_color(color.toBuilder<false>())
                .set_total(total)
                .set_max_hosts(max_hosts.toBuilder<false>());
    }

    uint32_t fields_mask;
    long time_shift;
    Dictionary_String tags;
    Function what;
    String name;
    String color;
    int total;
    ArrayString<false> max_hosts;
};

auto domainSeriesMeta()
{
    return fuzztest::StructOf<SeriesMeta>(fuzztest::InRange<uint32_t>(0, 3),
                                          fuzztest::Arbitrary<long>(),
                                          domainDictionary_String(),
                                          domainFunction(),
                                          domainString(),
                                          domainString(),
                                          fuzztest::Arbitrary<int>(),
                                          domainArrayString<false>());
}



template <bool INNER_BOXED>
struct ArraySeriesMeta {
    static constexpr Nat MIN_MASK_VALUE = 0;
    static constexpr Nat MAX_MASK_VALUE = 511;

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::SeriesMeta_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::SeriesMeta_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<SeriesMeta> arr;
};

template <bool INNER_BOXED>
auto domainArraySeriesMeta()
{
    return fuzztest::StructOf<ArraySeriesMeta<INNER_BOXED>>(fuzztest::VectorOf(domainSeriesMeta()).WithMaxSize(64));
}

}    // namespace fuzz_details
