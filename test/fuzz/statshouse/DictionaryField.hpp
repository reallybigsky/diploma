#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/String.hpp"



namespace fuzz_details {

struct DictionaryField_String {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::statshouse::DictionaryField_BASE<BOXED, ::string>::Builder {}
                .set_key(key.toBuilder<false>())
                .set_value(value.toBuilder<false>());
    }

    String key;
    String value;
};

auto domainDictionaryField_String()
{
    return fuzztest::StructOf<DictionaryField_String>(domainString(),
                                                      domainString());
};



template <bool INNER_BOXED>
struct ArrayDictionaryField_String {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename ::statshouse::DictionaryField_BASE<INNER_BOXED, ::string>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, ::statshouse::DictionaryField_BASE<INNER_BOXED, ::string>>::Builder {}
                .setArray(tmp);
    }

    std::vector<DictionaryField_String> arr;
};

template <bool INNER_BOXED>
auto domainArrayDictionaryField_String()
{
    return fuzztest::StructOf<ArrayDictionaryField_String<INNER_BOXED>>(fuzztest::VectorOf(domainDictionaryField_String()).WithMaxSize(128));
}

}    // namespace fuzz_details
