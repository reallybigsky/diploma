#include "fuzztest/fuzztest.h"
#include <gtest/gtest.h>

#include "test/fuzz/FuzzBlockStream.hpp"

#include "test/fuzz/statshouse/Dictionary.hpp"
#include "test/fuzz/statshouse/DictionaryField.hpp"

#include "test/fuzz/statshouse/api/GetChunkResponse.hpp"
#include "test/fuzz/statshouse/api/GetQueryPointResponse.hpp"
#include "test/fuzz/statshouse/api/GetQueryResponse.hpp"
#include "test/fuzz/statshouse/api/Query.hpp"
#include "test/fuzz/statshouse/api/QueryPoint.hpp"
#include "test/fuzz/statshouse/api/ReleaseChunkResponse.hpp"
#include "test/fuzz/statshouse/engine/HttpQuery.hpp"
#include "test/fuzz/statshouse/engine/HttpQueryResponse.hpp"
#include "test/fuzz/statshouse/public/AddMetricsBatchQuery.hpp"
#include "test/fuzz/statshouse/schema/SourceBucket2.hpp"

#include "test/fuzz/statshouse/api/Filter.hpp"
#include "test/fuzz/statshouse/api/Flag.hpp"
#include "test/fuzz/statshouse/api/Function.hpp"
#include "test/fuzz/statshouse/api/PointMeta.hpp"
#include "test/fuzz/statshouse/api/Series.hpp"
#include "test/fuzz/statshouse/api/SeriesMeta.hpp"
#include "test/fuzz/statshouse/api/TagValue.hpp"
#include "test/fuzz/statshouse/public/Metric.hpp"
#include "test/fuzz/statshouse/schema/Centroid.hpp"
#include "test/fuzz/statshouse/schema/IngestionStatus2.hpp"
#include "test/fuzz/statshouse/schema/MultiItem.hpp"
#include "test/fuzz/statshouse/schema/MultiValue.hpp"
#include "test/fuzz/statshouse/schema/SampleFactor.hpp"
#include "test/fuzz/statshouse/schema/TopElement.hpp"



auto StatshouseDomain()
{
    return fuzztest::VariantOf(
            fuzz_details::domainDictionaryField_String(),
            fuzz_details::domainArrayDictionaryField_String<true>(),
            fuzz_details::domainArrayDictionaryField_String<false>(),

            fuzz_details::domainDictionary_String(),
            fuzz_details::domainArrayDictionary_String<true>(),
            fuzz_details::domainArrayDictionary_String<false>(),

            fuzz_details::domainMetric(),
            fuzz_details::domainArrayMetric<true>(),
            fuzz_details::domainArrayMetric<false>(),

            fuzz_details::domainFunction(),
            fuzz_details::domainArrayFunction(),

            fuzz_details::domainFlag(),
            fuzz_details::domainArrayFlag(),

            fuzz_details::domainTagValue(),
            fuzz_details::domainArrayTagValue<true>(),
            fuzz_details::domainArrayTagValue<false>(),

            fuzz_details::domainFilter(),
            fuzz_details::domainArrayFilter<true>(),
            fuzz_details::domainArrayFilter<false>(),

            fuzz_details::domainSeries(),
            fuzz_details::domainArraySeries<true>(),
            fuzz_details::domainArraySeries<false>(),

            fuzz_details::domainSeriesMeta(),
            fuzz_details::domainArraySeriesMeta<true>(),
            fuzz_details::domainArraySeriesMeta<false>(),

            fuzz_details::domainPointMeta(),
            fuzz_details::domainArrayPointMeta<true>(),
            fuzz_details::domainArrayPointMeta<false>(),

            fuzz_details::domainCentroid(),
            fuzz_details::domainArrayCentroid<true>(),
            fuzz_details::domainArrayCentroid<false>(),

            fuzz_details::domainMultiValue(),
            fuzz_details::domainArrayMultiValue<true>(),
            fuzz_details::domainArrayMultiValue<false>(),

            fuzz_details::domainTopElement(),
            fuzz_details::domainArrayTopElement<true>(),
            fuzz_details::domainArrayTopElement<false>(),

            fuzz_details::domainMultiItem(),
            fuzz_details::domainArrayMultiItem<true>(),
            fuzz_details::domainArrayMultiItem<false>(),

            fuzz_details::domainSampleFactor(),
            fuzz_details::domainArraySampleFactor<true>(),
            fuzz_details::domainArraySampleFactor<false>(),

            fuzz_details::domainIngestionStatus2(),
            fuzz_details::domainArrayIngestionStatus2<true>(),
            fuzz_details::domainArrayIngestionStatus2<false>()

    );
};

using StatshouseVariant = std::variant<
        fuzz_details::DictionaryField_String,
        fuzz_details::ArrayDictionaryField_String<true>,
        fuzz_details::ArrayDictionaryField_String<false>,

        fuzz_details::Dictionary_String,
        fuzz_details::ArrayDictionary_String<true>,
        fuzz_details::ArrayDictionary_String<false>,

        fuzz_details::Metric,
        fuzz_details::ArrayMetric<true>,
        fuzz_details::ArrayMetric<false>,

        fuzz_details::Function,
        fuzz_details::ArrayFunction,

        fuzz_details::Flag,
        fuzz_details::ArrayFlag,

        fuzz_details::TagValue,
        fuzz_details::ArrayTagValue<true>,
        fuzz_details::ArrayTagValue<false>,

        fuzz_details::Filter,
        fuzz_details::ArrayFilter<true>,
        fuzz_details::ArrayFilter<false>,

        fuzz_details::Series,
        fuzz_details::ArraySeries<true>,
        fuzz_details::ArraySeries<false>,

        fuzz_details::SeriesMeta,
        fuzz_details::ArraySeriesMeta<true>,
        fuzz_details::ArraySeriesMeta<false>,

        fuzz_details::PointMeta,
        fuzz_details::ArrayPointMeta<true>,
        fuzz_details::ArrayPointMeta<false>,

        fuzz_details::Centroid,
        fuzz_details::ArrayCentroid<true>,
        fuzz_details::ArrayCentroid<false>,

        fuzz_details::MultiValue,
        fuzz_details::ArrayMultiValue<true>,
        fuzz_details::ArrayMultiValue<false>,

        fuzz_details::TopElement,
        fuzz_details::ArrayTopElement<true>,
        fuzz_details::ArrayTopElement<false>,

        fuzz_details::MultiItem,
        fuzz_details::ArrayMultiItem<true>,
        fuzz_details::ArrayMultiItem<false>,

        fuzz_details::SampleFactor,
        fuzz_details::ArraySampleFactor<true>,
        fuzz_details::ArraySampleFactor<false>,

        fuzz_details::IngestionStatus2,
        fuzz_details::ArrayIngestionStatus2<true>,
        fuzz_details::ArrayIngestionStatus2<false>

        >;



auto HighLevelTypeDomain()
{
    return fuzztest::VariantOf(
            fuzz_details::domainAddMetricsBatchQuery(),
            fuzz_details::domainHttpQuery(),
            fuzz_details::domainHttpQueryResponse(),
            fuzz_details::domainQuery(),
            fuzz_details::domainQueryPoint(),
            fuzz_details::domainGetQueryResponse(),
            fuzz_details::domainGetQueryPointResponse(),
            fuzz_details::domainGetChunkResponse(),
            fuzz_details::domainReleaseChunkResponse(),
            fuzz_details::domainSourceBucket2());
}

using HighLevelTypeVariant = std::variant<
        fuzz_details::AddMetricsBatchQuery,
        fuzz_details::HttpQuery,
        fuzz_details::HttpQueryResponse,
        fuzz_details::Query,
        fuzz_details::QueryPoint,
        fuzz_details::GetQueryResponse,
        fuzz_details::GetQueryPointResponse,
        fuzz_details::GetChunkResponse,
        fuzz_details::ReleaseChunkResponse,
        fuzz_details::SourceBucket2>;


class StatshouseFuzzBlockStream : public FuzzBlockStream {
public:
    StatshouseFuzzBlockStream()
        : FuzzBlockStream()
    {}

    void storeFetch(bool magic, const StatshouseVariant& input)
    {
        Allocator::startScope();
        checkBlockStreamSize();
        std::visit([&](auto&& arg) {
            if (magic) {
                doStoreFetch<true>(arg);
            } else {
                doStoreFetch<true>(arg);
            }
        },
                   input);
        Allocator::endScope();
    }

    template <bool MAGIC, typename ARG>
    void doStoreFetch(ARG&& arg)
    requires(!requires { std::decay_t<ARG>::MIN_MASK_VALUE; std::decay_t<ARG>::MAX_MASK_VALUE; })
    {
        auto builder = arg.template toBuilder<true>();
        using TL_TYPE = std::decay_t<decltype(builder)>::TYPE;

        builder.store(os);

        auto result = TL_TYPE::fetch(is);
        ASSERT_EQ(result, builder);
        ASSERT_TRUE(os.pos() % 4 == 0);
        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        auto new_result = TL_TYPE::fetch(is);
        ASSERT_EQ(result, new_result);
        ASSERT_TRUE(os.pos() % 4 == 0);
        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }

    template <bool MAGIC, typename ARG>
    void doStoreFetch(ARG&& arg)
    requires(requires { std::decay_t<ARG>::MIN_MASK_VALUE; std::decay_t<ARG>::MAX_MASK_VALUE; })
    {
        auto builder = arg.template toBuilder<true>();
        using TL_TYPE = std::decay_t<decltype(builder)>::TYPE;
        Nat mask = utils::random_mask<std::decay_t<ARG>::MIN_MASK_VALUE, std::decay_t<ARG>::MAX_MASK_VALUE>(DEFAULT_RANDOM_ENGINE);

        builder.store(os, mask);

        auto result = TL_TYPE::fetch(is, mask);
        ASSERT_EQ(result, builder);
        ASSERT_TRUE(os.pos() % 4 == 0);
        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os, mask);
        auto new_result = TL_TYPE::fetch(is, mask);
        ASSERT_EQ(result, new_result);
        ASSERT_TRUE(os.pos() % 4 == 0);
        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }

    void highLevelFuzz(const HighLevelTypeVariant& input)
    {
        Allocator::startScope();
        checkBlockStreamSize();
        std::visit([&](auto&& arg) {
            auto builder = arg.toBuilder();
            builder.store(os);

            ::statshouse::HighLevelType result = ::statshouse::HighLevelType::fetch(is);
            std::visit([&](auto&& inner) {
                if constexpr (std::is_same_v<typename std::decay_t<decltype(inner)>::Builder, std::decay_t<decltype(builder)>>) {
                    ASSERT_EQ(inner, builder);
                }
                ASSERT_TRUE(os.pos() % 4 == 0);
                ASSERT_EQ(os.pos(), is.pos());
                ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
                testCopyMove(inner);
            },
                       result.get());

            result.store(os);
            ::statshouse::HighLevelType new_result = ::statshouse::HighLevelType::fetch(is);
            ASSERT_EQ(result, new_result);
            ASSERT_TRUE(os.pos() % 4 == 0);
            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        },
                   input);
        Allocator::endScope();
    }
};



FUZZ_TEST_F(StatshouseFuzzBlockStream, storeFetch)
        .WithDomains(
                fuzztest::Arbitrary<bool>(),
                StatshouseDomain());

FUZZ_TEST_F(StatshouseFuzzBlockStream, highLevelFuzz)
        .WithDomains(HighLevelTypeDomain());