#pragma once

#include "test/Utils.hpp"

static_assert(simple::compoundRTS::MAGIC != 0);



template <typename T>
class CompoundRTSFixture : public testing::Test {
public:
    void SetUp() override
    {
        Allocator::startScope();
    }

    void TearDown() override
    {
        Allocator::endScope();
    }

    static constexpr size_t ARRAY_INT_SIZE = 100;
    static constexpr size_t STRING1_SIZE = 512;
    static constexpr size_t STRINGINTLONG_SIZE = 512;
    static constexpr size_t STRING2_SIZE = 512;
    static constexpr size_t ARRAY_INTDOUBLELONG_SIZE = 10;
};

using CompoundRTSMagic = ::testing::Types<std::true_type, std::false_type>;
TYPED_TEST_SUITE(CompoundRTSFixture, CompoundRTSMagic);



TYPED_TEST(CompoundRTSFixture, Single)
{
    using CompoundRTST = simple::CompoundRTS_BASE<TypeParam::value>;
    using Builder = typename CompoundRTST::Builder;

    const size_t size = 6144;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder = Builder::template random<TestFixture::ARRAY_INT_SIZE,
                                               TestFixture::STRING1_SIZE,
                                               TestFixture::STRINGINTLONG_SIZE,
                                               TestFixture::STRING2_SIZE,
                                               TestFixture::ARRAY_INTDOUBLELONG_SIZE>(DEFAULT_RANDOM_ENGINE);
    builder.store(os);

    CompoundRTST result = CompoundRTST::fetch(is);
    ASSERT_EQ(result, builder);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    CompoundRTST new_result = CompoundRTST::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(CompoundRTSFixture, Couple)
{
    using CompoundRTST = simple::CompoundRTS_BASE<TypeParam::value>;
    using Builder = typename CompoundRTST::Builder;

    const size_t size = 20000;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder1 = Builder::template random<TestFixture::ARRAY_INT_SIZE,
                                                TestFixture::STRING1_SIZE,
                                                TestFixture::STRINGINTLONG_SIZE,
                                                TestFixture::STRING2_SIZE,
                                                TestFixture::ARRAY_INTDOUBLELONG_SIZE>(DEFAULT_RANDOM_ENGINE);
    builder1.store(os);

    Builder builder2 = Builder::template random<TestFixture::ARRAY_INT_SIZE,
                                                TestFixture::STRING1_SIZE,
                                                TestFixture::STRINGINTLONG_SIZE,
                                                TestFixture::STRING2_SIZE,
                                                TestFixture::ARRAY_INTDOUBLELONG_SIZE>(DEFAULT_RANDOM_ENGINE);
    builder2.store(os);

    Builder builder3 = Builder::template random<TestFixture::ARRAY_INT_SIZE,
                                                TestFixture::STRING1_SIZE,
                                                TestFixture::STRINGINTLONG_SIZE,
                                                TestFixture::STRING2_SIZE,
                                                TestFixture::ARRAY_INTDOUBLELONG_SIZE>(DEFAULT_RANDOM_ENGINE);
    builder3.store(os);

    CompoundRTST result1 = CompoundRTST::fetch(is);
    ASSERT_EQ(result1, builder1);

    CompoundRTST result2 = CompoundRTST::fetch(is);
    ASSERT_EQ(result2, builder2);

    CompoundRTST result3 = CompoundRTST::fetch(is);
    ASSERT_EQ(result3, builder3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    CompoundRTST new_result1 = CompoundRTST::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    CompoundRTST new_result2 = CompoundRTST::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    CompoundRTST new_result3 = CompoundRTST::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(CompoundRTSFixture, SingleWithOverlap)
{
    using CompoundRTST = simple::CompoundRTS_BASE<TypeParam::value>;
    using Builder = typename CompoundRTST::Builder;

    const size_t size = 32;
    const size_t N = 200;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < N; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    Builder builder = Builder::template random<TestFixture::ARRAY_INT_SIZE,
                                               TestFixture::STRING1_SIZE,
                                               TestFixture::STRINGINTLONG_SIZE,
                                               TestFixture::STRING2_SIZE,
                                               TestFixture::ARRAY_INTDOUBLELONG_SIZE>(DEFAULT_RANDOM_ENGINE);
    ;
    builder.store(os);

    CompoundRTST result = CompoundRTST::fetch(is);
    ASSERT_EQ(result, builder);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    CompoundRTST new_result = CompoundRTST::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(CompoundRTSFixture, CoupleWithOverlap)
{
    using CompoundRTST = simple::CompoundRTS_BASE<TypeParam::value>;
    using Builder = typename CompoundRTST::Builder;

    const size_t size = 48;
    const size_t N = 400;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < N; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    Builder builder1 = Builder::template random<TestFixture::ARRAY_INT_SIZE,
                                                TestFixture::STRING1_SIZE,
                                                TestFixture::STRINGINTLONG_SIZE,
                                                TestFixture::STRING2_SIZE,
                                                TestFixture::ARRAY_INTDOUBLELONG_SIZE>(DEFAULT_RANDOM_ENGINE);
    builder1.store(os);

    Builder builder2 = Builder::template random<TestFixture::ARRAY_INT_SIZE,
                                                TestFixture::STRING1_SIZE,
                                                TestFixture::STRINGINTLONG_SIZE,
                                                TestFixture::STRING2_SIZE,
                                                TestFixture::ARRAY_INTDOUBLELONG_SIZE>(DEFAULT_RANDOM_ENGINE);
    builder2.store(os);

    Builder builder3 = Builder::template random<TestFixture::ARRAY_INT_SIZE,
                                                TestFixture::STRING1_SIZE,
                                                TestFixture::STRINGINTLONG_SIZE,
                                                TestFixture::STRING2_SIZE,
                                                TestFixture::ARRAY_INTDOUBLELONG_SIZE>(DEFAULT_RANDOM_ENGINE);
    builder3.store(os);

    CompoundRTST result1 = CompoundRTST::fetch(is);
    ASSERT_EQ(result1, builder1);

    CompoundRTST result2 = CompoundRTST::fetch(is);
    ASSERT_EQ(result2, builder2);

    CompoundRTST result3 = CompoundRTST::fetch(is);
    ASSERT_EQ(result3, builder3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    CompoundRTST new_result1 = CompoundRTST::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    CompoundRTST new_result2 = CompoundRTST::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    CompoundRTST new_result3 = CompoundRTST::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(CompoundRTSFixture, Random)
{
    using CompoundRTST = simple::CompoundRTS_BASE<TypeParam::value>;
    using Builder = typename CompoundRTST::Builder;

    const size_t size = 64;
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
        Builder builder = Builder::template random<TestFixture::ARRAY_INT_SIZE,
                                                   TestFixture::STRING1_SIZE,
                                                   TestFixture::STRINGINTLONG_SIZE,
                                                   TestFixture::STRING2_SIZE,
                                                   TestFixture::ARRAY_INTDOUBLELONG_SIZE>(DEFAULT_RANDOM_ENGINE);
        builder.store(os);

        CompoundRTST result = CompoundRTST::fetch(is);
        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        CompoundRTST new_result = CompoundRTST::fetch(is);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(CompoundRTSFixture, Array)
{
    using CompoundRTST = simple::CompoundRTS_BASE<TypeParam::value>;

    const size_t size = 128;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 250000; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 128; ++i) {
        using ArrayBuilder = array<CompoundRTST>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10,
                                                                 TestFixture::ARRAY_INT_SIZE,
                                                                 TestFixture::STRING1_SIZE,
                                                                 TestFixture::STRINGINTLONG_SIZE,
                                                                 TestFixture::STRING2_SIZE,
                                                                 TestFixture::ARRAY_INTDOUBLELONG_SIZE>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<CompoundRTST> elems;
        {
            array<CompoundRTST> result = array<CompoundRTST>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            array<CompoundRTST> new_result = array<CompoundRTST>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}

TEST(CompoundRTS, WrongMagic)
{
    Allocator::startScope();
    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    String::Builder builder;
    builder.store(os);

    ASSERT_THROW(simple::CompoundRTS::fetch(is), TLException);
    Allocator::endScope();
}