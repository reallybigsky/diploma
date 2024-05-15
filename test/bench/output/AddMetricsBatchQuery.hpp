#pragma once

#include "test/Environment.hpp"



// 4 + 4 + 128 × (4 + 1024 + 128 × 2 × 1024 + 8 + 4 + 8 × 1024 + 8 × 1024)
// 35 784 712
statshouse::AddMetricsBatchQuery::Builder createAddMetricsBatchQuery(std::default_random_engine& engine) noexcept
{
//    static constexpr size_t MAX_METRIC_SIZE = 64;
//    static constexpr size_t MAX_METRIC_NAME_SIZE = 1024;
//    static constexpr size_t MAX_METRIC_TAGS_DICTIONARY_SIZE = 128;
//    static constexpr size_t MAX_METRIC_TAGS_DICTIONARY_KEY_SIZE = 1024;
//    static constexpr size_t MAX_METRIC_TAGS_DICTIONARY_VALUE_SIZE = 1024;
//    static constexpr size_t MAX_METRIC_VALUE_ARRAY_SIZE = 1024;
//    static constexpr size_t MAX_METRIC_UNIQUE_ARRAY_SIZE = 1024;
    static constexpr size_t MAX_METRIC_SIZE = 256;
    static constexpr size_t MAX_METRIC_NAME_SIZE = 128;
    static constexpr size_t MAX_METRIC_TAGS_DICTIONARY_SIZE = 128;
    static constexpr size_t MAX_METRIC_TAGS_DICTIONARY_KEY_SIZE = 128;
    static constexpr size_t MAX_METRIC_TAGS_DICTIONARY_VALUE_SIZE = 128;
    static constexpr size_t MAX_METRIC_VALUE_ARRAY_SIZE = 1024;
    static constexpr size_t MAX_METRIC_UNIQUE_ARRAY_SIZE = 1024;
    return statshouse::AddMetricsBatchQuery::Builder::random<MAX_METRIC_SIZE,
                                                             MAX_METRIC_NAME_SIZE,
                                                             MAX_METRIC_TAGS_DICTIONARY_SIZE,
                                                             MAX_METRIC_TAGS_DICTIONARY_KEY_SIZE,
                                                             MAX_METRIC_TAGS_DICTIONARY_VALUE_SIZE,
                                                             MAX_METRIC_VALUE_ARRAY_SIZE,
                                                             MAX_METRIC_UNIQUE_ARRAY_SIZE>(engine);
}