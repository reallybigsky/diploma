#pragma once

#include "test/Environment.hpp"



// 4 + 4 + 1024 × 8 + 32 × (4 + 8 + 8 + 8 + 128 × 2 × 1024 + 4)
// 8 397 832
statshouse::GetQueryPointResponse::Builder createGetQueryPointResponse(std::default_random_engine& engine) noexcept
{
    static constexpr size_t MAX_DATA_ARRAY_SIZE = 1024;
    static constexpr size_t MAX_META_ARRAY_SIZE = 32;
    static constexpr size_t MAX_META_ELEMENT_TAGS_DICTIONARY_SIZE = 128;
    static constexpr size_t MAX_META_ELEMENT_TAGS_DICTIONARY_KEY_SIZE = 1024;
    static constexpr size_t MAX_META_ELEMENT_TAGS_DICTIONARY_VALUE_SIZE = 1024;
    return statshouse::GetQueryPointResponse::Builder::random<MAX_DATA_ARRAY_SIZE,
                                                              MAX_META_ARRAY_SIZE,
                                                              MAX_META_ELEMENT_TAGS_DICTIONARY_SIZE,
                                                              MAX_META_ELEMENT_TAGS_DICTIONARY_KEY_SIZE,
                                                              MAX_META_ELEMENT_TAGS_DICTIONARY_VALUE_SIZE
                                                              >(engine);
}
