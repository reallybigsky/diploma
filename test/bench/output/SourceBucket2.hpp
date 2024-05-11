#pragma once

#include "test/Environment.hpp"



// 4 + 32 ×(4 + 4 + 1024 × 4 + 4 + 8 + 8 + 8 + 8 + 8 + 1024 + 1024 × 8 + 4 + 4 +4 + 32 × (1024 + 4 + 8 +8 +8+8+8 + 1024 + 1024 × 8 + 4 +4 +4)) + 1024 × 8 + 1024 × 8 + 4 + 4 + 1024 × 12// 2 878 476
// 10 999 820
statshouse::SourceBucket2::Builder createSourceBucket2(std::default_random_engine& engine) noexcept
{
    static constexpr size_t MAX_METRICS_ARRAY_SIZE = 32;
    static constexpr size_t MAX_METRICS_ELEMENT_KEYS_ARRAY_SIZE = 1024;
    static constexpr size_t MAX_METRICS_ELEMENT_TAIL_UNIQUES_SIZE = 1024;
    static constexpr size_t MAX_METRICS_ELEMENT_TAIL_CENTROIDS_ARRAY_SIZE = 1024;
    static constexpr size_t MAX_METRICS_ELEMENT_TOP_ARRAY_SIZE = 32;
    static constexpr size_t MAX_METRICS_ELEMENT_TOP_ELEMENT_KEY_SIZE = 1024;
    static constexpr size_t MAX_METRICS_ELEMENT_TOP_ELEMENT_VALUE_UNIQUES_SIZE = 1024;
    static constexpr size_t MAX_METRICS_ELEMENT_TOP_ELEMENT_VALUE_CENTROIDS_ARRAY_SIZE = 1024;
    static constexpr size_t MAX_SAMPLE_FACTORS_ARRAY_SIZE = 1024;
    static constexpr size_t MAX_INGESTION_STATUS_OK_ARRAY_SIZE = 1024;
    static constexpr size_t MAX_INGESTION_STATUS_OK2_ARRAY_SIZE = 1024;
    return statshouse::SourceBucket2::Builder::random<MAX_METRICS_ARRAY_SIZE,
                                                      MAX_METRICS_ELEMENT_KEYS_ARRAY_SIZE,
                                                      MAX_METRICS_ELEMENT_TAIL_UNIQUES_SIZE,
                                                      MAX_METRICS_ELEMENT_TAIL_CENTROIDS_ARRAY_SIZE,
                                                      MAX_METRICS_ELEMENT_TOP_ARRAY_SIZE,
                                                      MAX_METRICS_ELEMENT_TOP_ELEMENT_KEY_SIZE,
                                                      MAX_METRICS_ELEMENT_TOP_ELEMENT_VALUE_UNIQUES_SIZE,
                                                      MAX_METRICS_ELEMENT_TOP_ELEMENT_VALUE_CENTROIDS_ARRAY_SIZE,
                                                      MAX_SAMPLE_FACTORS_ARRAY_SIZE,
                                                      MAX_INGESTION_STATUS_OK_ARRAY_SIZE,
                                                      MAX_INGESTION_STATUS_OK2_ARRAY_SIZE>(engine);
}
