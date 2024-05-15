#pragma once

#include "test/Environment.hpp"



// 4 + 4 + 4 + 1024 × 1024 + 256 + 128 × 2 × 1024
// 1 310 988
statshouse::HttpQueryResponse::Builder createHttpQueryResponse(std::default_random_engine& engine) noexcept
{
//    static constexpr size_t MAX_DATA_SIZE = 16 * 1024 * 1024;
//    static constexpr size_t MAX_CONTENT_TYPE_SIZE = 256;
//    static constexpr size_t MAX_ADDITIONAL_HEADERS_DICTIONARY_SIZE = 128;
//    static constexpr size_t MAX_ADDITIONAL_HEADERS_DICTIONARY_KEY_SIZE = 1024;
//    static constexpr size_t MAX_ADDITIONAL_HEADERS_DICTIONARY_VALUE_SIZE = 1024;
    static constexpr size_t MAX_DATA_SIZE = 16 * 1024 * 1024;
    static constexpr size_t MAX_CONTENT_TYPE_SIZE = 128;
    static constexpr size_t MAX_ADDITIONAL_HEADERS_DICTIONARY_SIZE = 128;
    static constexpr size_t MAX_ADDITIONAL_HEADERS_DICTIONARY_KEY_SIZE = 128;
    static constexpr size_t MAX_ADDITIONAL_HEADERS_DICTIONARY_VALUE_SIZE = 128;
    return statshouse::HttpQueryResponse::Builder::random<MAX_DATA_SIZE,
                                                          MAX_CONTENT_TYPE_SIZE,
                                                          MAX_ADDITIONAL_HEADERS_DICTIONARY_SIZE,
                                                          MAX_ADDITIONAL_HEADERS_DICTIONARY_KEY_SIZE,
                                                          MAX_ADDITIONAL_HEADERS_DICTIONARY_VALUE_SIZE>(engine);
}
