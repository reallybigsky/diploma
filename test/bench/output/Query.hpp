#pragma once

#include "test/Environment.hpp"



// 4 + 4 + 4 + 4 + 1024 + 8 + 8 + 1024 + 4 + 64 × 1024 + 128 × (4 + 1024 + 128 × (4 + 4 + 1024+ 4)) + 8 × 1024 + 1024 + 4 × 1024 + 1024
// 17 187 364
statshouse::Query::Builder createQuery(std::default_random_engine& engine) noexcept
{
//    static constexpr size_t MAX_METRIC_NAME_SIZE = 256;
//    static constexpr size_t MAX_INTERVAL_SIZE = 256;
//    static constexpr size_t MAX_GROUP_BY_ARRAY_SIZE = 128;
//    static constexpr size_t MAX_GROUP_BY_ELEMENT_SIZE = 1024;
//    static constexpr size_t MAX_FILTERS_ARRAY_SIZE = 256;
//    static constexpr size_t MAX_FILTERS_KEY_ELEMENT_SIZE = 1024;
//    static constexpr size_t MAX_FILTERS_VALUES_ELEMENT_ARRAY_SIZE = 256;
//    static constexpr size_t MAX_FILTERS_VALUES_ELEMENT_VALUES_VALUE_SIZE = 1024;
//    static constexpr size_t MAX_TIME_SHIFT_ARRAY_SIZE = 256;
//    static constexpr size_t MAX_PROMQL_SIZE = 1024;
//    static constexpr size_t MAX_WHAT_ARRAY_SIZE = 256;
//    static constexpr size_t MAX_WIDTHAGG_SIZE = 1024;
    static constexpr size_t MAX_METRIC_NAME_SIZE = 128;
    static constexpr size_t MAX_INTERVAL_SIZE = 256;
    static constexpr size_t MAX_GROUP_BY_ARRAY_SIZE = 128;
    static constexpr size_t MAX_GROUP_BY_ELEMENT_SIZE = 128;
    static constexpr size_t MAX_FILTERS_ARRAY_SIZE = 128;
    static constexpr size_t MAX_FILTERS_KEY_ELEMENT_SIZE = 128;
    static constexpr size_t MAX_FILTERS_VALUES_ELEMENT_ARRAY_SIZE = 128;
    static constexpr size_t MAX_FILTERS_VALUES_ELEMENT_VALUES_VALUE_SIZE = 256;
    static constexpr size_t MAX_TIME_SHIFT_ARRAY_SIZE = 128;
    static constexpr size_t MAX_PROMQL_SIZE = 1024;
    static constexpr size_t MAX_WHAT_ARRAY_SIZE = 128;
    static constexpr size_t MAX_WIDTHAGG_SIZE = 128;
    return statshouse::Query::Builder::random<MAX_METRIC_NAME_SIZE,
                                              MAX_INTERVAL_SIZE,
                                              MAX_GROUP_BY_ARRAY_SIZE,
                                              MAX_GROUP_BY_ELEMENT_SIZE,
                                              MAX_FILTERS_ARRAY_SIZE,
                                              MAX_FILTERS_KEY_ELEMENT_SIZE,
                                              MAX_FILTERS_VALUES_ELEMENT_ARRAY_SIZE,
                                              MAX_FILTERS_VALUES_ELEMENT_VALUES_VALUE_SIZE,
                                              MAX_TIME_SHIFT_ARRAY_SIZE,
                                              MAX_PROMQL_SIZE,
                                              MAX_WHAT_ARRAY_SIZE,
                                              MAX_WIDTHAGG_SIZE>(engine);
}
