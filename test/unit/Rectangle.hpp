#pragma once

#include "test/Utils.hpp"



static_assert(simple::rectangle_s<0>::SIZEOF == 0);
static_assert(simple::rectangle_s<1>::SIZEOF == 16);
static_assert(simple::rectangle_s<2>::SIZEOF == 16);
static_assert(simple::rectangle_s<4>::SIZEOF == 16);
static_assert(simple::rectangle_s<3>::SIZEOF == 32);
static_assert(simple::rectangle_s<5>::SIZEOF == 32);
static_assert(simple::rectangle_s<6>::SIZEOF == 32);
static_assert(simple::rectangle_s<7>::SIZEOF == 48);

static_assert(simple::Rectangle_s<0>::SIZEOF == 4);
static_assert(simple::Rectangle_s<1>::SIZEOF == 20);
static_assert(simple::Rectangle_s<2>::SIZEOF == 20);
static_assert(simple::Rectangle_s<4>::SIZEOF == 20);
static_assert(simple::Rectangle_s<3>::SIZEOF == 36);
static_assert(simple::Rectangle_s<5>::SIZEOF == 36);
static_assert(simple::Rectangle_s<6>::SIZEOF == 36);
static_assert(simple::Rectangle_s<7>::SIZEOF == 52);



template <typename T>
class RectangleFixture : public testing::Test {
public:
    static constexpr size_t MASK_MIN = 0;
    static constexpr size_t MASK_MAX = 8;
};

using RectangleMagic = ::testing::Types<std::true_type, std::false_type>;
TYPED_TEST_SUITE(RectangleFixture, RectangleMagic);



template <typename T>
class RectangleStaticMaskFixture : public testing::Test {};

using RectangleStaticMask = ::testing::Types<std::integral_constant<uint32_t, 0>,
                                             std::integral_constant<uint32_t, 1>,
                                             std::integral_constant<uint32_t, 2>,
                                             std::integral_constant<uint32_t, 3>,
                                             std::integral_constant<uint32_t, 4>,
                                             std::integral_constant<uint32_t, 5>,
                                             std::integral_constant<uint32_t, 6>,
                                             std::integral_constant<uint32_t, 7>>;
TYPED_TEST_SUITE(RectangleStaticMaskFixture, RectangleStaticMask);


TYPED_TEST(RectangleFixture, Single)
{
    using RectangleT = simple::Rectangle_BASE<TypeParam::value>;
    using Builder = typename RectangleT::Builder;

    for (size_t mask = TestFixture::MASK_MIN; mask < TestFixture::MASK_MAX; ++mask) {
        const size_t size = 1024;
        auto block = createBlock(size);
        OutputStream os = {block};
        InputStream is = {block};

        Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder.store(os, mask);

        RectangleT result = RectangleT::fetch(is, mask);

        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os, mask);
        RectangleT new_result = RectangleT::fetch(is, mask);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(RectangleFixture, Couple)
{
    using RectangleT = simple::Rectangle_BASE<TypeParam::value>;
    using Builder = typename RectangleT::Builder;

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

        RectangleT result1 = RectangleT::fetch(is, mask);
        ASSERT_EQ(result1, builder1);

        RectangleT result2 = RectangleT::fetch(is, mask);
        ASSERT_EQ(result2, builder2);

        RectangleT result3 = RectangleT::fetch(is, mask);
        ASSERT_EQ(result3, builder3);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result1);
        testCopyMove(result2);
        testCopyMove(result3);

        result1.store(os, mask);
        RectangleT new_result1 = RectangleT::fetch(is, mask);
        ASSERT_EQ(result1, new_result1);

        result2.store(os, mask);
        RectangleT new_result2 = RectangleT::fetch(is, mask);
        ASSERT_EQ(result2, new_result2);

        result3.store(os, mask);
        RectangleT new_result3 = RectangleT::fetch(is, mask);
        ASSERT_EQ(result3, new_result3);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(RectangleFixture, SingleWithOverlap)
{
    using RectangleT = simple::Rectangle_BASE<TypeParam::value>;
    using Builder = typename RectangleT::Builder;

    for (size_t mask = TestFixture::MASK_MIN; mask < TestFixture::MASK_MAX; ++mask) {
        const size_t size = 32;
        DataStream::Block block1 = createBlock(size);
        DataStream::Block block2 = createBlock(size);
        DataStream::Block block3 = createBlock(size);
        DataStream::Block block4 = createBlock(size);
        OutputStream os = {block1, block2, block3, block4};
        InputStream is = {block1, block2, block3, block4};

        Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder.store(os, mask);

        RectangleT result = RectangleT::fetch(is, mask);

        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os, mask);
        RectangleT new_result = RectangleT::fetch(is, mask);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(RectangleFixture, CoupleWithOverlap)
{
    using RectangleT = simple::Rectangle_BASE<TypeParam::value>;
    using Builder = typename RectangleT::Builder;

    for (size_t mask = TestFixture::MASK_MIN; mask < TestFixture::MASK_MAX; ++mask) {
        const size_t size = 32;
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

        RectangleT result1 = RectangleT::fetch(is, mask);
        ASSERT_EQ(result1, builder1);

        RectangleT result2 = RectangleT::fetch(is, mask);
        ASSERT_EQ(result2, builder2);

        RectangleT result3 = RectangleT::fetch(is, mask);
        ASSERT_EQ(result3, builder3);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result1);
        testCopyMove(result2);
        testCopyMove(result3);

        result1.store(os, mask);
        RectangleT new_result1 = RectangleT::fetch(is, mask);
        ASSERT_EQ(result1, new_result1);

        result2.store(os, mask);
        RectangleT new_result2 = RectangleT::fetch(is, mask);
        ASSERT_EQ(result2, new_result2);

        result3.store(os, mask);
        RectangleT new_result3 = RectangleT::fetch(is, mask);
        ASSERT_EQ(result3, new_result3);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(RectangleFixture, Random)
{
    using RectangleT = simple::Rectangle_BASE<TypeParam::value>;
    using Builder = typename RectangleT::Builder;

    for (size_t mask = TestFixture::MASK_MIN; mask < TestFixture::MASK_MAX; ++mask) {
        const size_t size = 32;
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

            RectangleT result = RectangleT::fetch(is, mask);
            ASSERT_EQ(result, builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            result.store(os, mask);
            RectangleT new_result = RectangleT::fetch(is, mask);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }
    }
}

TYPED_TEST(RectangleFixture, Array)
{
    using RectangleT = simple::Rectangle_BASE<TypeParam::value>;

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
            using ArrayBuilder = array<RectangleT>::Builder;
            ArrayBuilder arr_builder = ArrayBuilder::template random<10>(DEFAULT_RANDOM_ENGINE);
            arr_builder.store(os, mask);

            std::vector<RectangleT> elems;
            {
                array<RectangleT> result = array<RectangleT>::fetch(is, mask);
                ASSERT_EQ(result, arr_builder);

                ASSERT_EQ(os.pos(), is.pos());
                ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

                testCopyMove(result);

                elems.insert(elems.end(), result.begin(), result.end());

                result.store(os, mask);
                array<RectangleT> new_result = array<RectangleT>::fetch(is, mask);
                ASSERT_EQ(result, new_result);

                ASSERT_EQ(os.pos(), is.pos());
                ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
            }

            testCopyMoveIteration(elems);
        }
    }
}

TYPED_TEST(RectangleStaticMaskFixture, MagicSingle)
{
    using RectangleT = simple::Rectangle_BASE<true, TypeParam::value>;
    using Builder = typename RectangleT::Builder;

    const size_t size = 1024;
    auto block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder.store(os);

    RectangleT result = RectangleT::fetch(is);

    ASSERT_EQ(result, builder);

    ASSERT_EQ(is.pos(), RectangleT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    RectangleT new_result = RectangleT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(is.pos(), 2 * RectangleT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(RectangleStaticMaskFixture, Single)
{
    using RectangleT = simple::Rectangle_BASE<false, TypeParam::value>;
    using Builder = typename RectangleT::Builder;

    const size_t size = 1024;
    auto block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder.store(os);

    RectangleT result = RectangleT::fetch(is);

    ASSERT_EQ(result, builder);

    ASSERT_EQ(is.pos(), RectangleT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    RectangleT new_result = RectangleT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(is.pos(), 2 * RectangleT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(RectangleStaticMaskFixture, MagicCouple)
{
    using RectangleT = simple::Rectangle_BASE<true, TypeParam::value>;
    using Builder = typename RectangleT::Builder;

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

    RectangleT result1 = RectangleT::fetch(is);
    ASSERT_EQ(result1, builder1);

    RectangleT result2 = RectangleT::fetch(is);
    ASSERT_EQ(result2, builder2);

    RectangleT result3 = RectangleT::fetch(is);
    ASSERT_EQ(result3, builder3);

    ASSERT_EQ(is.pos(), 3 * RectangleT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    RectangleT new_result1 = RectangleT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    RectangleT new_result2 = RectangleT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    RectangleT new_result3 = RectangleT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(is.pos(), 6 * RectangleT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(RectangleStaticMaskFixture, Couple)
{
    using RectangleT = simple::Rectangle_BASE<false, TypeParam::value>;
    using Builder = typename RectangleT::Builder;

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

    RectangleT result1 = RectangleT::fetch(is);
    ASSERT_EQ(result1, builder1);

    RectangleT result2 = RectangleT::fetch(is);
    ASSERT_EQ(result2, builder2);

    RectangleT result3 = RectangleT::fetch(is);
    ASSERT_EQ(result3, builder3);

    ASSERT_EQ(is.pos(), 3 * RectangleT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    RectangleT new_result1 = RectangleT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    RectangleT new_result2 = RectangleT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    RectangleT new_result3 = RectangleT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(is.pos(), 6 * RectangleT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(RectangleStaticMaskFixture, MagicSingleWithOverlap)
{
    using RectangleT = simple::Rectangle_BASE<true, TypeParam::value>;
    using Builder = typename RectangleT::Builder;

    const size_t size = 32;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder.store(os);

    RectangleT result = RectangleT::fetch(is);

    ASSERT_EQ(result, builder);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    RectangleT new_result = RectangleT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(RectangleStaticMaskFixture, SingleWithOverlap)
{
    using RectangleT = simple::Rectangle_BASE<false, TypeParam::value>;
    using Builder = typename RectangleT::Builder;

    const size_t size = 32;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder.store(os);

    RectangleT result = RectangleT::fetch(is);

    ASSERT_EQ(result, builder);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    RectangleT new_result = RectangleT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(RectangleStaticMaskFixture, MagicCoupleWithOverlap)
{
    using RectangleT = simple::Rectangle_BASE<true, TypeParam::value>;
    using Builder = typename RectangleT::Builder;

    const size_t size = 32;
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

    RectangleT result1 = RectangleT::fetch(is);
    ASSERT_EQ(result1, builder1);

    RectangleT result2 = RectangleT::fetch(is);
    ASSERT_EQ(result2, builder2);

    RectangleT result3 = RectangleT::fetch(is);
    ASSERT_EQ(result3, builder3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    RectangleT new_result1 = RectangleT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    RectangleT new_result2 = RectangleT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    RectangleT new_result3 = RectangleT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(RectangleStaticMaskFixture, CoupleWithOverlap)
{
    using RectangleT = simple::Rectangle_BASE<false, TypeParam::value>;
    using Builder = typename RectangleT::Builder;

    const size_t size = 32;
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

    RectangleT result1 = RectangleT::fetch(is);
    ASSERT_EQ(result1, builder1);

    RectangleT result2 = RectangleT::fetch(is);
    ASSERT_EQ(result2, builder2);

    RectangleT result3 = RectangleT::fetch(is);
    ASSERT_EQ(result3, builder3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    RectangleT new_result1 = RectangleT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    RectangleT new_result2 = RectangleT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    RectangleT new_result3 = RectangleT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(RectangleStaticMaskFixture, MagicRandom)
{
    using RectangleT = simple::Rectangle_BASE<true, TypeParam::value>;
    using Builder = typename RectangleT::Builder;

    const size_t size = 32;
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

        RectangleT result = RectangleT::fetch(is);
        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        RectangleT new_result = RectangleT::fetch(is);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(RectangleStaticMaskFixture, Random)
{
    using RectangleT = simple::Rectangle_BASE<false, TypeParam::value>;
    using Builder = typename RectangleT::Builder;

    const size_t size = 32;
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

        RectangleT result = RectangleT::fetch(is);
        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        RectangleT new_result = RectangleT::fetch(is);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(RectangleStaticMaskFixture, MagicArray)
{
    using RectangleT = simple::Rectangle_BASE<true, TypeParam::value>;

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
        using ArrayBuilder = array<RectangleT>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<RectangleT> elems;
        {
            array<RectangleT> result = array<RectangleT>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            array<RectangleT> new_result = array<RectangleT>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}

TYPED_TEST(RectangleStaticMaskFixture, Array)
{
    using RectangleT = simple::Rectangle_BASE<false, TypeParam::value>;

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
        using ArrayBuilder = array<RectangleT>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<RectangleT> elems;
        {
            array<RectangleT> result = array<RectangleT>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            array<RectangleT> new_result = array<RectangleT>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}
