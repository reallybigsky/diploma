#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/String.hpp"
#include "test/fuzz/statshouse/Dictionary.hpp"


namespace fuzz_details {

struct HttpQueryResponse {

    auto toBuilder() const noexcept
    {
        return typename ::statshouse::HttpQueryResponse::Builder {}
                .set_fields_mask(fields_mask)
                .set_return_code(return_code)
                .set_data(data.toBuilder<false>())
                .set_content_type(content_type.toBuilder<false>())
                .set_additional_headers(additional_headers.toBuilder<false>());
    }

    uint32_t fields_mask;
    int return_code;
    String data;
    String content_type;
    Dictionary_String additional_headers;
};

auto domainHttpQueryResponse()
{
    return fuzztest::StructOf<HttpQueryResponse>(fuzztest::InRange<uint32_t>(0, 15),
                                                 fuzztest::Arbitrary<int>(),
                                                 domainString(),
                                                 domainString(),
                                                 domainDictionary_String());
}

}    // namespace fuzz_details