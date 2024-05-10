#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/String.hpp"
#include "test/fuzz/statshouse/Dictionary.hpp"


namespace fuzz_details {

struct HttpQuery {

    auto toBuilder() const noexcept
    {
        return typename ::statshouse::HttpQuery::Builder {}
                .set_fields_mask(fields_mask)
                .set_uri(uri.toBuilder<false>())
                .set_args(args.toBuilder<false>())
                .set_headers(headers.toBuilder<false>());
    }

    uint32_t fields_mask;
    String uri;
    Dictionary_String args;
    Dictionary_String headers;
};

auto domainHttpQuery()
{
    return fuzztest::StructOf<HttpQuery>(fuzztest::InRange<uint32_t>(0, 7),
                                         domainString(),
                                         domainDictionary_String(),
                                         domainDictionary_String());
}

}    // namespace fuzz_details