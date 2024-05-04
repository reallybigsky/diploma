#pragma once

#include "test/Utils.hpp"

static_assert(simple::StringIntLong::MAGIC != 0);



template <typename T>
class PictureFixture : public testing::Test {};

using PictureMagic = ::testing::Types<std::true_type, std::false_type>;
TYPED_TEST_SUITE(PictureFixture, PictureMagic);



TYPED_TEST(PictureFixture, Single)
{
    using PictureT = simple::Picture_BASE<TypeParam::value>;
    using Builder = typename PictureT::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder.store(os);

    PictureT result = PictureT::fetch(is);
    ASSERT_EQ(result, builder);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    PictureT new_result = PictureT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(PictureFixture, Couple)
{
    using PictureT = simple::Picture_BASE<TypeParam::value>;
    using Builder = typename PictureT::Builder;

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

    PictureT result1 = PictureT::fetch(is);
    ASSERT_EQ(result1, builder1);

    PictureT result2 = PictureT::fetch(is);
    ASSERT_EQ(result2, builder2);

    PictureT result3 = PictureT::fetch(is);
    ASSERT_EQ(result3, builder3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    PictureT new_result1 = PictureT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    PictureT new_result2 = PictureT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    PictureT new_result3 = PictureT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(PictureFixture, SingleWithOverlap)
{
    using PictureT = simple::Picture_BASE<TypeParam::value>;
    using Builder = typename PictureT::Builder;

    const size_t size = 36;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder.store(os);

    PictureT result = PictureT::fetch(is);
    ASSERT_EQ(result, builder);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    PictureT new_result = PictureT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(PictureFixture, CoupleWithOverlap)
{
    using PictureT = simple::Picture_BASE<TypeParam::value>;
    using Builder = typename PictureT::Builder;

    const size_t size = 32;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;
    for (size_t i = 0; i < 16; ++i) {
        auto block = createBlock(size);
        is.add(block);
        os.add(block);
        storage.emplace_back(block);
    }

    Builder builder1 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder1.store(os);

    Builder builder2 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder2.store(os);

    Builder builder3 = Builder::random(DEFAULT_RANDOM_ENGINE);
    builder3.store(os);

    PictureT result1 = PictureT::fetch(is);
    ASSERT_EQ(result1, builder1);

    PictureT result2 = PictureT::fetch(is);
    ASSERT_EQ(result2, builder2);

    PictureT result3 = PictureT::fetch(is);
    ASSERT_EQ(result3, builder3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    PictureT new_result1 = PictureT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    PictureT new_result2 = PictureT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    PictureT new_result3 = PictureT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(PictureFixture, Random)
{
    using PictureT = simple::Picture_BASE<TypeParam::value>;
    using Builder = typename PictureT::Builder;

    const size_t size = 16;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 10000; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 1000; ++i) {
        Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder.store(os);

        PictureT result = PictureT::fetch(is);
        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        PictureT new_result = PictureT::fetch(is);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(PictureFixture, Array)
{
    using PictureT = simple::Picture_BASE<TypeParam::value>;

    const size_t size = 192;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 10000; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 128; ++i) {
        using ArrayBuilder = array<PictureT>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<PictureT> elems;
        {
            array<PictureT> result = array<PictureT>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            array<PictureT> new_result = array<PictureT>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}
