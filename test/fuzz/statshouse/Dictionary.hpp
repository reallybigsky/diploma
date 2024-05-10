#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/statshouse/DictionaryField.hpp"



namespace fuzz_details {

struct Dictionary_String {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::statshouse::Dictionary_BASE<BOXED, ::string>::Builder {}
                .set_array1(array1.toBuilder<false>());
    }

    ArrayDictionaryField_String<false> array1;
};

auto domainDictionary_String()
{
    return fuzztest::StructOf<Dictionary_String>(domainArrayDictionaryField_String<false>());
}



template <bool INNER_BOXED>
struct ArrayDictionary_String {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::Dictionary_BASE<INNER_BOXED, ::string>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::Dictionary_BASE<INNER_BOXED, ::string>>::Builder {}
                .setArray(tmp);
    }

    std::vector<Dictionary_String> arr;
};

template <bool INNER_BOXED>
auto domainArrayDictionary_String()
{
    return fuzztest::StructOf<ArrayDictionary_String<INNER_BOXED>>(fuzztest::VectorOf(domainDictionary_String()).WithMaxSize(16));
}

}    // namespace fuzz_details
