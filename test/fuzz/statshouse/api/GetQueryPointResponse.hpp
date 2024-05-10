#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/ArrayPrimitive.hpp"
#include "test/fuzz/statshouse/api/PointMeta.hpp"



namespace fuzz_details {

struct GetQueryPointResponse {

    auto toBuilder() const noexcept
    {
        return typename ::statshouse::GetQueryPointResponse::Builder {}
                .set_fields_mask(fields_mask)
                .set_data(data.toBuilder<false>())
                .set_meta(meta.toBuilder<false>());
    }

    uint32_t fields_mask;
    ArrayPrimitive<double> data;
    ArrayPointMeta<false> meta;
};

auto domainGetQueryPointResponse()
{
    return fuzztest::StructOf<GetQueryPointResponse>(fuzztest::Arbitrary<uint32_t>(),
                                                     domainArrayPrimitive<double>(),
                                                     domainArrayPointMeta<false>());
}

}    // namespace fuzz_details
