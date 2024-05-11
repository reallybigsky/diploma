#pragma once

#include "test/Environment.hpp"



// 4 + 4 +  4 + 1024 × 1024× 8 + 1024 × 8 + 32 × (4 + 4 + 128 × 2 × 1024 + 4 + 1024 + 1024 + 4 + 1024 × 1024) + 4×1024 + 4 + 8
// 50 410 008
statshouse::GetQueryResponse::Builder createGetQueryResponse(std::default_random_engine& engine) noexcept
{
    static constexpr size_t MAX_SERIES_SERIES_DATA_ARRAY_SIZE = 1024;
    static constexpr size_t MAX_SERIES_SERIES_DATA_ELEMENT_ARRAY_SIE = 1024;
    static constexpr size_t MAX_SERIES_TIME_ARRAY_SIZE = 1024;
    static constexpr size_t MAX_SERIES_META_ARRAY_SIZE = 32;
    static constexpr size_t MAX_SERIES_META_ELEMENT_TAGS_DICTIONARY_SIZE = 128;
    static constexpr size_t MAX_SERIES_META_ELEMENT_TAGS_DICTIONARY_KEY_SIZE = 1024;
    static constexpr size_t MAX_SERIES_META_ELEMENT_TAGS_DICTIONARY_VALUE_SIZE = 1024;
    static constexpr size_t MAX_SERIES_META_ELEMENT_NAME_SIZE = 1024;
    static constexpr size_t MAX_SERIES_META_ELEMENT_COLOR_SIZE = 1024;
    static constexpr size_t MAX_SERIES_META_ELEMENT_MAX_HOSTS_ARRAY_SIZE = 1024;
    static constexpr size_t MAX_SERIES_META_ELEMENT_MAX_HOSTS_ELEMENT_SIZE = 1024;
    static constexpr size_t MAX_CHUNK_IDS_ARRAY_SIZE = 1024;
    return statshouse::GetQueryResponse::Builder::random<MAX_SERIES_SERIES_DATA_ARRAY_SIZE,
                                                         MAX_SERIES_SERIES_DATA_ELEMENT_ARRAY_SIE,
                                                         MAX_SERIES_TIME_ARRAY_SIZE,
                                                         MAX_SERIES_META_ARRAY_SIZE,
                                                         MAX_SERIES_META_ELEMENT_TAGS_DICTIONARY_SIZE,
                                                         MAX_SERIES_META_ELEMENT_TAGS_DICTIONARY_KEY_SIZE,
                                                         MAX_SERIES_META_ELEMENT_TAGS_DICTIONARY_VALUE_SIZE,
                                                         MAX_SERIES_META_ELEMENT_NAME_SIZE,
                                                         MAX_SERIES_META_ELEMENT_COLOR_SIZE,
                                                         MAX_SERIES_META_ELEMENT_MAX_HOSTS_ARRAY_SIZE,
                                                         MAX_SERIES_META_ELEMENT_MAX_HOSTS_ELEMENT_SIZE,
                                                         MAX_CHUNK_IDS_ARRAY_SIZE>(engine);
}
