#pragma once

#include "test/Utils.hpp"

static_assert(simple::intDouble::MAGIC != 0);
static_assert(simple::intDouble::SIZEOF == 12);
static_assert(simple::IntDouble::SIZEOF == 16);



template <typename T>
class IntDoubleFixture : public testing::Test {
public:
    void SetUp() override
    {
        Allocator::startScope();
    }

    void TearDown() override
    {
        Allocator::endScope();
    }
};

using IntDoubleMagic = ::testing::Types<std::true_type, std::false_type>;
TYPED_TEST_SUITE(IntDoubleFixture, IntDoubleMagic);



TYPED_TEST(IntDoubleFixture, Single)
{
    using IntDoubleT = simple::IntDouble_BASE<TypeParam::value>;
    using Builder = typename IntDoubleT::Builder;

    const size_t size = 1024;
    auto block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder;
    builder.set_Int1(42)
            .set_Double1(13.37)
            .store(os);

    IntDoubleT result = IntDoubleT::fetch(is);
    ASSERT_EQ(result, builder);
    ASSERT_EQ(result.get_Int1(), 42);
    ASSERT_EQ(result.get_Double1(), 13.37);

    ASSERT_EQ(is.pos(), IntDoubleT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    IntDoubleT new_result = IntDoubleT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(is.pos(), 2 * IntDoubleT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(IntDoubleFixture, Couple)
{
    using IntDoubleT = simple::IntDouble_BASE<TypeParam::value>;
    using Builder = typename IntDoubleT::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder1;
    builder1.set_Int1(111)
            .set_Double1(22.22)
            .store(os);

    Builder builder2;
    builder2.set_Int1(333)
            .set_Double1(44.44)
            .store(os);

    Builder builder3;
    builder3.set_Int1(555)
            .set_Double1(66.66)
            .store(os);

    IntDoubleT result1 = IntDoubleT::fetch(is);
    ASSERT_EQ(result1, builder1);
    ASSERT_EQ(result1.get_Int1(), 111);
    ASSERT_EQ(result1.get_Double1(), 22.22);

    IntDoubleT result2 = IntDoubleT::fetch(is);
    ASSERT_EQ(result2, builder2);
    ASSERT_EQ(result2.get_Int1(), 333);
    ASSERT_EQ(result2.get_Double1(), 44.44);

    IntDoubleT result3 = IntDoubleT::fetch(is);
    ASSERT_EQ(result3, builder3);
    ASSERT_EQ(result3.get_Int1(), 555);
    ASSERT_EQ(result3.get_Double1(), 66.66);

    ASSERT_EQ(is.pos(), 3 * IntDoubleT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    IntDoubleT new_result1 = IntDoubleT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    IntDoubleT new_result2 = IntDoubleT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    IntDoubleT new_result3 = IntDoubleT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(is.pos(), 6 * IntDoubleT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(IntDoubleFixture, SingleWithOverlap)
{
    using IntDoubleT = simple::IntDouble_BASE<TypeParam::value>;
    using Builder = typename IntDoubleT::Builder;

    const size_t size = 8;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    Builder builder;
    builder.set_Int1(42)
            .set_Double1(13.37)
            .store(os);

    IntDoubleT result = IntDoubleT::fetch(is);
    ASSERT_EQ(result, builder);
    ASSERT_EQ(result.get_Int1(), 42);
    ASSERT_EQ(result.get_Double1(), 13.37);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    IntDoubleT new_result = IntDoubleT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(IntDoubleFixture, CoupleWithOverlap)
{
    using IntDoubleT = simple::IntDouble_BASE<TypeParam::value>;
    using Builder = typename IntDoubleT::Builder;

    const size_t size = 24;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    Builder builder1;
    builder1.set_Int1(111)
            .set_Double1(22.22)
            .store(os);

    Builder builder2;
    builder2.set_Int1(333)
            .set_Double1(44.44)
            .store(os);

    Builder builder3;
    builder3.set_Int1(555)
            .set_Double1(66.66)
            .store(os);

    IntDoubleT result1 = IntDoubleT::fetch(is);
    ASSERT_EQ(result1, builder1);
    ASSERT_EQ(result1.get_Int1(), 111);
    ASSERT_EQ(result1.get_Double1(), 22.22);

    IntDoubleT result2 = IntDoubleT::fetch(is);
    ASSERT_EQ(result2, builder2);
    ASSERT_EQ(result2.get_Int1(), 333);
    ASSERT_EQ(result2.get_Double1(), 44.44);

    IntDoubleT result3 = IntDoubleT::fetch(is);
    ASSERT_EQ(result3, builder3);
    ASSERT_EQ(result3.get_Int1(), 555);
    ASSERT_EQ(result3.get_Double1(), 66.66);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    IntDoubleT new_result1 = IntDoubleT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    IntDoubleT new_result2 = IntDoubleT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    IntDoubleT new_result3 = IntDoubleT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(IntDoubleFixture, Random)
{
    using IntDoubleT = simple::IntDouble_BASE<TypeParam::value>;
    using Builder = typename IntDoubleT::Builder;

    const size_t size = 20;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 100; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 50; ++i) {
        Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder.store(os);

        IntDoubleT result = IntDoubleT::fetch(is);
        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        IntDoubleT new_result = IntDoubleT::fetch(is);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(IntDoubleFixture, Array)
{
    using IntDoubleT = simple::IntDouble_BASE<TypeParam::value>;

    const size_t size = 8;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 16000; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 300; ++i) {
        using ArrayBuilder = array<IntDoubleT>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<IntDoubleT> elems;
        {
            array<IntDoubleT> result = array<IntDoubleT>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            array<IntDoubleT> new_result = array<IntDoubleT>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}

TYPED_TEST(IntDoubleFixture, ArrayMagic)
{
    using IntDoubleT = simple::IntDouble_BASE<TypeParam::value>;

    const size_t size = 8;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 16000; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 1; ++i) {
        using ArrayBuilder = Array<IntDoubleT>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<IntDoubleT> elems;
        {
            Array<IntDoubleT> result = Array<IntDoubleT>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            Array<IntDoubleT> new_result = Array<IntDoubleT>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}

TEST(IntDouble, WrongMagic)
{
    Allocator::startScope();
    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    String::Builder builder;
    builder.store(os);

    ASSERT_THROW(simple::IntDouble::fetch(is), TLException);
    Allocator::endScope();
}