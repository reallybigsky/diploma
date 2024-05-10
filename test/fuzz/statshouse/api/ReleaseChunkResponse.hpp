#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"



namespace fuzz_details {

struct ReleaseChunkResponse {

    auto toBuilder() const noexcept
    {
        return typename ::statshouse::ReleaseChunkResponse::Builder {}
                .set_fields_mask(fields_mask)
                .set_releasedChunkCount(releasedChunkCount);
    }

    uint32_t fields_mask;
    int releasedChunkCount;
};

auto domainReleaseChunkResponse()
{
    return fuzztest::StructOf<ReleaseChunkResponse>(fuzztest::Arbitrary<uint32_t>(),
                                                    fuzztest::Arbitrary<int>());
}

}    // namespace fuzz_details
