#pragma once

#include "fuzztest/fuzztest.h"

#include "Environment.hpp"

#include "fuzz/String.hpp"

#include "baseline/statshouse/Dictionary.hpp"



namespace fuzz_details {

struct DictionaryFieldString {
    using Type = ::statshouse::DictionaryField<::string>;
    using Builder = Type::Builder;

    Builder toBuilder() const noexcept
    {
        Builder result;
        result.set_key(s_key.toBuilder())
                .set_value(s_value.toBuilder());

        return result;
    }

    string s_key;
    string s_value;
};

auto domainDictionaryFieldString()
{
    return fuzztest::StructOf<DictionaryFieldString>(domainString(),
                                                     domainString());
}



struct DictionaryString {
    using Type = ::statshouse::Dictionary<::string>;
    using Builder = Type::Builder;

    Builder toBuilder() const noexcept
    {
        Builder result;

        std::vector<::statshouse::DictionaryField<::string>::Builder> arr;
        arr.reserve(s_dict.size());
        for (const auto& value : s_dict) {
            arr.emplace_back(value.toBuilder());
        }

        result.set_data(arr);

        return result;
    }

    std::vector<DictionaryFieldString> s_dict;
};

auto domainDictionaryString()
{
    return fuzztest::StructOf<DictionaryString>(fuzztest::VectorOf(domainDictionaryFieldString()).WithMaxSize(1000));
}

}    // namespace fuzz_details
