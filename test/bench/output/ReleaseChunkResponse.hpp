#pragma once

#include "test/Environment.hpp"



// 4 + 4 + 4
// 12
statshouse::ReleaseChunkResponse::Builder createReleaseChunkResponse(std::default_random_engine& engine) noexcept
{
    return statshouse::ReleaseChunkResponse::Builder::random(engine);
}
