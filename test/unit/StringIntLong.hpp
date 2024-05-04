#pragma once

#include "test/Utils.hpp"

static_assert(simple::StringIntLong::MAGIC != 0);



template <typename T>
class StringIntLongFixture : public testing::Test {};

using StringIntLongMagic = ::testing::Types<std::true_type, std::false_type>;
TYPED_TEST_SUITE(StringIntLongFixture, StringIntLongMagic);



TYPED_TEST(StringIntLongFixture, Single)
{
    using StringIntLongT = simple::StringIntLong_BASE<TypeParam::value>;
    using Builder = typename StringIntLongT::Builder;

    const size_t size = 2048;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder = Builder::template random<512>(DEFAULT_RANDOM_ENGINE);
    builder.store(os);

    StringIntLongT result = StringIntLongT::fetch(is);
    ASSERT_EQ(result, builder);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    StringIntLongT new_result = StringIntLongT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(StringIntLongFixture, Couple)
{
    using StringIntLongT = simple::StringIntLong_BASE<TypeParam::value>;
    using Builder = typename StringIntLongT::Builder;

    const size_t size = 4096;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder1 = Builder::template random<512>(DEFAULT_RANDOM_ENGINE);
    builder1.store(os);

    Builder builder2 = Builder::template random<512>(DEFAULT_RANDOM_ENGINE);
    builder2.store(os);

    Builder builder3 = Builder::template random<512>(DEFAULT_RANDOM_ENGINE);
    builder3.store(os);

    StringIntLongT result1 = StringIntLongT::fetch(is);
    ASSERT_EQ(result1, builder1);

    StringIntLongT result2 = StringIntLongT::fetch(is);
    ASSERT_EQ(result2, builder2);

    StringIntLongT result3 = StringIntLongT::fetch(is);
    ASSERT_EQ(result3, builder3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    StringIntLongT new_result1 = StringIntLongT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    StringIntLongT new_result2 = StringIntLongT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    StringIntLongT new_result3 = StringIntLongT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(StringIntLongFixture, SingleWithOverlap)
{
    using StringIntLongT = simple::StringIntLong_BASE<TypeParam::value>;
    using Builder = typename StringIntLongT::Builder;

    const size_t size = 384;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    Builder builder = Builder::template random<512>(DEFAULT_RANDOM_ENGINE);
    builder.store(os);

    StringIntLongT result = StringIntLongT::fetch(is);
    ASSERT_EQ(result, builder);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    StringIntLongT new_result = StringIntLongT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(StringIntLongFixture, CoupleWithOverlap)
{
    using StringIntLongT = simple::StringIntLong_BASE<TypeParam::value>;
    using Builder = typename StringIntLongT::Builder;

    const size_t size = 1024;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    Builder builder1 = Builder::template random<512>(DEFAULT_RANDOM_ENGINE);
    builder1.store(os);

    Builder builder2 = Builder::template random<512>(DEFAULT_RANDOM_ENGINE);
    builder2.store(os);

    Builder builder3 = Builder::template random<512>(DEFAULT_RANDOM_ENGINE);
    builder3.store(os);

    StringIntLongT result1 = StringIntLongT::fetch(is);
    ASSERT_EQ(result1, builder1);

    StringIntLongT result2 = StringIntLongT::fetch(is);
    ASSERT_EQ(result2, builder2);

    StringIntLongT result3 = StringIntLongT::fetch(is);
    ASSERT_EQ(result3, builder3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    StringIntLongT new_result1 = StringIntLongT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    StringIntLongT new_result2 = StringIntLongT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    StringIntLongT new_result3 = StringIntLongT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(StringIntLongFixture, Random)
{
    using StringIntLongT = simple::StringIntLong_BASE<TypeParam::value>;
    using Builder = typename StringIntLongT::Builder;

    const size_t size = 1024;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 2000; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 768; ++i) {
        Builder builder = Builder::template random<1024>(DEFAULT_RANDOM_ENGINE);
        builder.store(os);

        StringIntLongT result = StringIntLongT::fetch(is);
        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        StringIntLongT new_result = StringIntLongT::fetch(is);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(StringIntLongFixture, Array)
{
    using StringIntLongT = simple::StringIntLong_BASE<TypeParam::value>;

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
        using ArrayBuilder = array<StringIntLongT>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10, 256>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<StringIntLongT> elems;
        {
            array<StringIntLongT> result = array<StringIntLongT>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            array<StringIntLongT> new_result = array<StringIntLongT>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}

TEST(StringIntLong, WrongMagic)
{
    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    String::Builder builder;
    builder.store(os);

    ASSERT_THROW(simple::StringIntLong::fetch(is), TLException);
}
