#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/ArrayPrimitive.hpp"
#include "test/fuzz/statshouse/api/Function.hpp"



namespace fuzz_details {

struct PointMeta {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::statshouse::PointMeta_BASE<BOXED>::Builder {}
                .set_fields_mask(fields_mask)
                .set_time_shift(time_shift)
                .set_from(from)
                .set_to(to)
                .set_tags(tags.toBuilder<false>())
                .set_what(what.toBuilder<>());
    }

    uint32_t fields_mask;
    long time_shift;
    long from;
    long to;
    Dictionary_String tags;
    Function what;
};

auto domainPointMeta()
{
    return fuzztest::StructOf<PointMeta>(fuzztest::InRange<uint32_t>(0, 3),
                                         fuzztest::Arbitrary<long>(),
                                         fuzztest::Arbitrary<long>(),
                                         fuzztest::Arbitrary<long>(),
                                         domainDictionary_String(),
                                         domainFunction());
}



template <bool INNER_BOXED>
struct ArrayPointMeta {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::PointMeta_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::PointMeta_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<PointMeta> arr;
};

template <bool INNER_BOXED>
auto domainArrayPointMeta()
{
    return fuzztest::StructOf<ArrayPointMeta<INNER_BOXED>>(fuzztest::VectorOf(domainPointMeta()).WithMaxSize(64));
}

}    // namespace fuzz_details
