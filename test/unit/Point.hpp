#pragma once

#include "test/Utils.hpp"



static_assert(simple::point_s<0>::SIZEOF == 0);
static_assert(simple::point_s<1>::SIZEOF == 8);
static_assert(simple::point_s<2>::SIZEOF == 8);
static_assert(simple::point_s<4>::SIZEOF == 8);
static_assert(simple::point_s<3>::SIZEOF == 16);
static_assert(simple::point_s<5>::SIZEOF == 16);
static_assert(simple::point_s<6>::SIZEOF == 16);
static_assert(simple::point_s<7>::SIZEOF == 24);

static_assert(simple::Point_s<0>::SIZEOF == 4);
static_assert(simple::Point_s<1>::SIZEOF == 12);
static_assert(simple::Point_s<2>::SIZEOF == 12);
static_assert(simple::Point_s<4>::SIZEOF == 12);
static_assert(simple::Point_s<3>::SIZEOF == 20);
static_assert(simple::Point_s<5>::SIZEOF == 20);
static_assert(simple::Point_s<6>::SIZEOF == 20);
static_assert(simple::Point_s<7>::SIZEOF == 28);



template <typename T>
class PointFixture : public testing::Test {
public:
    static constexpr size_t MASK_MIN = 0;
    static constexpr size_t MASK_MAX = 8;
};

using PointMagic = ::testing::Types<std::true_type, std::false_type>;
TYPED_TEST_SUITE(PointFixture, PointMagic);



template <typename T>
class PointStaticMaskFixture : public testing::Test {};

using PointStaticMask = ::testing::Types<std::integral_constant<uint32_t, 0>,
                                         std::integral_constant<uint32_t, 1>,
                                         std::integral_constant<uint32_t, 2>,
                                         std::integral_constant<uint32_t, 3>,
                                         std::integral_constant<uint32_t, 4>,
                                         std::integral_constant<uint32_t, 5>,
                                         std::integral_constant<uint32_t, 6>,
                                         std::integral_constant<uint32_t, 7>>;
TYPED_TEST_SUITE(PointStaticMaskFixture, PointStaticMask);



TYPED_TEST(PointFixture, Single)
{
    using PointT = simple::Point_BASE<TypeParam::value>;
    using Builder = typename PointT::Builder;

    for (size_t mask = TestFixture::MASK_MIN; mask < TestFixture::MASK_MAX; ++mask) {
        const size_t size = 1024;
        auto block = createBlock(size);
        OutputStream os = {block};
        InputStream is = {block};

        Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder.store(os, mask);

        PointT result = PointT::fetch(is, mask);

        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os, mask);
        PointT new_result = PointT::fetch(is, mask);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(PointFixture, Couple)
{
    using PointT = simple::Point_BASE<TypeParam::value>;
    using Builder = typename PointT::Builder;

    for (size_t mask = TestFixture::MASK_MIN; mask < TestFixture::MASK_MAX; ++mask) {
        const size_t size = 1024;
        DataStream::Block block = createBlock(size);
        OutputStream os = {block};
        InputStream is = {block};

        Builder builder1 = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder1.store(os, mask);

        Builder builder2 = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder2.store(os, mask);

        Builder builder3 = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder3.store(os, mask);

        PointT result1 = PointT::fetch(is, mask);
        ASSERT_EQ(result1, builder1);

        PointT result2 = PointT::fetch(is, mask);
        ASSERT_EQ(result2, builder2);

        PointT result3 = PointT::fetch(is, mask);
        ASSERT_EQ(result3, builder3);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result1);
        testCopyMove(result2);
        testCopyMove(result3);

        result1.store(os, mask);
        PointT new_result1 = PointT::fetch(is, mask);
        ASSERT_EQ(result1, new_result1);

        result2.store(os, mask);
        PointT new_result2 = PointT::fetch(is, mask);
        ASSERT_EQ(result2, new_result2);

        result3.store(os, mask);
        PointT new_result3 = PointT::fetch(is, mask);
        ASSERT_EQ(result3, new_result3);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(PointFixture, SingleWithOverlap)
{
    using PointT = simple::Point_BASE<TypeParam::value>;
    using Builder = typename PointT::Builder;

    for (size_t mask = TestFixture::MASK_MIN; mask < TestFixture::MASK_MAX; ++mask) {
        const size_t size = 16;
        DataStream::Block block1 = createBlock(size);
        DataStream::Block block2 = createBlock(size);
        DataStream::Block block3 = createBlock(size);
        DataStream::Block block4 = createBlock(size);
        OutputStream os = {block1, block2, block3, block4};
        InputStream is = {block1, block2, block3, block4};

        Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder.store(os, mask);

        PointT result = PointT::fetch(is, mask);

        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os, mask);
        PointT new_result = PointT::fetch(is, mask);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(PointFixture, CoupleWithOverlap)
{
    using PointT = simple::Point_BASE<TypeParam::value>;
    using Builder = typename PointT::Builder;

    for (size_t mask = TestFixture::MASK_MIN; mask < TestFixture::MASK_MAX; ++mask) {
        const size_t size = 16;
        OutputStream os;
        InputStream is;
        std::vector<DataStream::Block> storage;
        for (size_t i = 0; i < 12; ++i) {
            auto block = createBlock(size);
            os.add(block);
            is.add(block);
            storage.emplace_back(block);
        }

        Builder builder1 = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder1.store(os, mask);

        Builder builder2 = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder2.store(os, mask);

        Builder builder3 = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder3.store(os, mask);

        PointT result1 = PointT::fetch(is, mask);
        ASSERT_EQ(result1, builder1);

        PointT result2 = PointT::fetch(is, mask);
        ASSERT_EQ(result2, builder2);

        PointT result3 = PointT::fetch(is, mask);
        ASSERT_EQ(result3, builder3);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result1);
        testCopyMove(result2);
        testCopyMove(result3);

        result1.store(os, mask);
        PointT new_result1 = PointT::fetch(is, mask);
        ASSERT_EQ(result1, new_result1);

        result2.store(os, mask);
        PointT new_result2 = PointT::fetch(is, mask);
        ASSERT_EQ(result2, new_result2);

        result3.store(os, mask);
        PointT new_result3 = PointT::fetch(is, mask);
        ASSERT_EQ(result3, new_result3);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(PointFixture, Random)
{
    using PointT = simple::Point_BASE<TypeParam::value>;
    using Builder = typename PointT::Builder;

    for (size_t mask = TestFixture::MASK_MIN; mask < TestFixture::MASK_MAX; ++mask) {
        const size_t size = 16;
        OutputStream os;
        InputStream is;
        std::vector<DataStream::Block> storage;
        for (size_t i = 0; i < 512; ++i) {
            auto block = createBlock(size);
            os.add(block);
            is.add(block);
            storage.emplace_back(block);
        }

        for (size_t i = 0; i < 128; ++i) {
            Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
            builder.store(os, mask);

            PointT result = PointT::fetch(is, mask);
            ASSERT_EQ(result, builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            result.store(os, mask);
            PointT new_result = PointT::fetch(is, mask);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }
    }
}

TYPED_TEST(PointFixture, Array)
{
    using PointT = simple::Point_BASE<TypeParam::value>;

    for (size_t mask = TestFixture::MASK_MIN; mask < TestFixture::MASK_MAX; ++mask) {
        const size_t size = 256;
        OutputStream os;
        InputStream is;
        std::vector<DataStream::Block> storage;

        for (size_t i = 0; i < 20560; ++i) {
            auto block = createBlock(size);
            os.add(block);
            is.add(block);
            storage.emplace_back(block);
        }

        for (size_t i = 0; i < 256; ++i) {
            using ArrayBuilder = array<PointT>::Builder;
            ArrayBuilder arr_builder = ArrayBuilder::template random<10>(DEFAULT_RANDOM_ENGINE);
            arr_builder.store(os, mask);

            std::vector<PointT> elems;
            {
                array<PointT> result = array<PointT>::fetch(is, mask);
                ASSERT_EQ(result, arr_builder);

                ASSERT_EQ(os.pos(), is.pos());
                ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

                testCopyMove(result);

                elems.insert(elems.end(), result.begin(), result.end());

                result.store(os, mask);
                array<PointT> new_result = array<PointT>::fetch(is, mask);
                ASSERT_EQ(result, new_result);

                ASSERT_EQ(os.pos(), is.pos());
                ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
            }

            testCopyMoveIteration(elems);
        }
    }
}

TYPED_TEST(PointStaticMaskFixture, MagicSingle)
{
    using PointT = simple::Point_BASE<true, TypeParam::value>;
    using Builder = typename PointT::Builder;

    const size_t size = 1024;
    auto block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder.store(os);

    PointT result = PointT::fetch(is);

    ASSERT_EQ(result, builder);

    ASSERT_EQ(is.pos(), PointT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    PointT new_result = PointT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(is.pos(), 2 * PointT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(PointStaticMaskFixture, Single)
{
    using PointT = simple::Point_BASE<false, TypeParam::value>;
    using Builder = typename PointT::Builder;

    const size_t size = 1024;
    auto block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder.store(os);

    PointT result = PointT::fetch(is);

    ASSERT_EQ(result, builder);

    ASSERT_EQ(is.pos(), PointT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    PointT new_result = PointT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(is.pos(), 2 * PointT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(PointStaticMaskFixture, MagicCouple)
{
    using PointT = simple::Point_BASE<true, TypeParam::value>;
    using Builder = typename PointT::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder1 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder1.store(os);

    Builder builder2 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder2.store(os);

    Builder builder3 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder3.store(os);

    PointT result1 = PointT::fetch(is);
    ASSERT_EQ(result1, builder1);

    PointT result2 = PointT::fetch(is);
    ASSERT_EQ(result2, builder2);

    PointT result3 = PointT::fetch(is);
    ASSERT_EQ(result3, builder3);

    ASSERT_EQ(is.pos(), 3 * PointT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    PointT new_result1 = PointT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    PointT new_result2 = PointT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    PointT new_result3 = PointT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(is.pos(), 6 * PointT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(PointStaticMaskFixture, Couple)
{
    using PointT = simple::Point_BASE<false, TypeParam::value>;
    using Builder = typename PointT::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder1 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder1.store(os);

    Builder builder2 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder2.store(os);

    Builder builder3 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder3.store(os);

    PointT result1 = PointT::fetch(is);
    ASSERT_EQ(result1, builder1);

    PointT result2 = PointT::fetch(is);
    ASSERT_EQ(result2, builder2);

    PointT result3 = PointT::fetch(is);
    ASSERT_EQ(result3, builder3);

    ASSERT_EQ(is.pos(), 3 * PointT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    PointT new_result1 = PointT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    PointT new_result2 = PointT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    PointT new_result3 = PointT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(is.pos(), 6 * PointT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(PointStaticMaskFixture, MagicSingleWithOverlap)
{
    using PointT = simple::Point_BASE<true, TypeParam::value>;
    using Builder = typename PointT::Builder;

    const size_t size = 16;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder.store(os);

    PointT result = PointT::fetch(is);

    ASSERT_EQ(result, builder);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    PointT new_result = PointT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(PointStaticMaskFixture, SingleWithOverlap)
{
    using PointT = simple::Point_BASE<false, TypeParam::value>;
    using Builder = typename PointT::Builder;

    const size_t size = 16;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder.store(os);

    PointT result = PointT::fetch(is);

    ASSERT_EQ(result, builder);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    PointT new_result = PointT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(PointStaticMaskFixture, MagicCoupleWithOverlap)
{
    using PointT = simple::Point_BASE<true, TypeParam::value>;
    using Builder = typename PointT::Builder;

    const size_t size = 16;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;
    for (size_t i = 0; i < 12; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    Builder builder1 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder1.store(os);

    Builder builder2 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder2.store(os);

    Builder builder3 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder3.store(os);

    PointT result1 = PointT::fetch(is);
    ASSERT_EQ(result1, builder1);

    PointT result2 = PointT::fetch(is);
    ASSERT_EQ(result2, builder2);

    PointT result3 = PointT::fetch(is);
    ASSERT_EQ(result3, builder3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    PointT new_result1 = PointT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    PointT new_result2 = PointT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    PointT new_result3 = PointT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(PointStaticMaskFixture, CoupleWithOverlap)
{
    using PointT = simple::Point_BASE<false, TypeParam::value>;
    using Builder = typename PointT::Builder;

    const size_t size = 16;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;
    for (size_t i = 0; i < 12; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    Builder builder1 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder1.store(os);

    Builder builder2 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder2.store(os);

    Builder builder3 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder3.store(os);

    PointT result1 = PointT::fetch(is);
    ASSERT_EQ(result1, builder1);

    PointT result2 = PointT::fetch(is);
    ASSERT_EQ(result2, builder2);

    PointT result3 = PointT::fetch(is);
    ASSERT_EQ(result3, builder3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    PointT new_result1 = PointT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    PointT new_result2 = PointT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    PointT new_result3 = PointT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(PointStaticMaskFixture, MagicRandom)
{
    using PointT = simple::Point_BASE<true, TypeParam::value>;
    using Builder = typename PointT::Builder;

    const size_t size = 16;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;
    for (size_t i = 0; i < 512; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 128; ++i) {
        Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder.store(os);

        PointT result = PointT::fetch(is);
        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        PointT new_result = PointT::fetch(is);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(PointStaticMaskFixture, Random)
{
    using PointT = simple::Point_BASE<false, TypeParam::value>;
    using Builder = typename PointT::Builder;

    const size_t size = 16;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;
    for (size_t i = 0; i < 512; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 128; ++i) {
        Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder.store(os);

        PointT result = PointT::fetch(is);
        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        PointT new_result = PointT::fetch(is);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(PointStaticMaskFixture, MagicArray)
{
    using PointT = simple::Point_BASE<true, TypeParam::value>;

    const size_t size = 16;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 20560; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 16; ++i) {
        using ArrayBuilder = array<PointT>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<PointT> elems;
        {
            array<PointT> result = array<PointT>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            array<PointT> new_result = array<PointT>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}

TYPED_TEST(PointStaticMaskFixture, Array)
{
    using PointT = simple::Point_BASE<false, TypeParam::value>;

    const size_t size = 16;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 20560; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 16; ++i) {
        using ArrayBuilder = array<PointT>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<PointT> elems;
        {
            array<PointT> result = array<PointT>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            array<PointT> new_result = array<PointT>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}