#pragma once

#include "test/Environment.hpp"



// 4 + 4 + 4 + 16 × 1024 × 8 + 1024 × 8 + 4
// 139 280
statshouse::GetChunkResponse::Builder createGetChunkResponse(std::default_random_engine& engine) noexcept
{
    static constexpr size_t MAX_SERIES_SERIES_DATA_ARRAY_SIZE = 16;
    static constexpr size_t MAX_SERIES_SERIES_DATA_ELEMENT_ARRAY_SIE = 1024;
    static constexpr size_t MAX_SERIES_TIME_ARRAY_SIZE = 1024;
    return statshouse::GetChunkResponse::Builder::random<MAX_SERIES_SERIES_DATA_ARRAY_SIZE,
                                                         MAX_SERIES_SERIES_DATA_ELEMENT_ARRAY_SIE,
                                                         MAX_SERIES_TIME_ARRAY_SIZE>(engine);
}
