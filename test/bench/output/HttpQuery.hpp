#pragma once

#include "test/Environment.hpp"



// 4 + 4 + 1024 + 128 × 2 × 1024 + 128 × 2 × 1024
// 525 320
statshouse::HttpQuery::Builder createHttpQuery(std::default_random_engine& engine) noexcept
{
    static constexpr size_t MAX_URI_SIZE = 1024;
    static constexpr size_t MAX_ARGS_DICTIONARY_SIZE = 128;
    static constexpr size_t MAX_ARGS_DICTIONARY_KEY_SIZE = 1024;
    static constexpr size_t MAX_ARGS_DICTIONARY_VALUE_SIZE = 1024;
    static constexpr size_t MAX_HEADERS_DICTIONARY_SIZE = 128;
    static constexpr size_t MAX_HEADERS_DICTIONARY_KEY_SIZE = 1024;
    static constexpr size_t MAX_HEADERS_DICTIONARY_VALUE_SIZE = 1024;
    return statshouse::HttpQuery::Builder::random<MAX_URI_SIZE,
                                                  MAX_ARGS_DICTIONARY_SIZE,
                                                  MAX_ARGS_DICTIONARY_KEY_SIZE,
                                                  MAX_ARGS_DICTIONARY_VALUE_SIZE,
                                                  MAX_HEADERS_DICTIONARY_SIZE,
                                                  MAX_HEADERS_DICTIONARY_KEY_SIZE,
                                                  MAX_HEADERS_DICTIONARY_VALUE_SIZE>(engine);
}
