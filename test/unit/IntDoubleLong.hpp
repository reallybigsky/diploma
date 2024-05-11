#pragma once

#include "test/Utils.hpp"

static_assert(simple::intDoubleLong::MAGIC != 0);
static_assert(simple::intDoubleLong::SIZEOF == 20);
static_assert(simple::IntDoubleLong::SIZEOF == 24);



template <typename T>
class IntDoubleLongFixture : public testing::Test {
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

using IntDoubleLongMagic = ::testing::Types<std::true_type, std::false_type>;
TYPED_TEST_SUITE(IntDoubleLongFixture, IntDoubleLongMagic);



TYPED_TEST(IntDoubleLongFixture, Single)
{
    using IntDoubleLongT = simple::IntDoubleLong_BASE<TypeParam::value>;
    using Builder = typename IntDoubleLongT::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder;
    builder.set_Int1(1436)
            .set_Double1(2562.3214213)
            .set_Long1(-143243215)
            .store(os);

    IntDoubleLongT result = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result, builder);
    ASSERT_EQ(result.get_Int1(), 1436);
    ASSERT_EQ(result.get_Double1(), 2562.3214213);
    ASSERT_EQ(result.get_Long1(), -143243215);

    ASSERT_EQ(is.pos(), IntDoubleLongT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    IntDoubleLongT new_result = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(is.pos(), 2 * IntDoubleLongT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(IntDoubleLongFixture, Couple)
{
    using IntDoubleLongT = simple::IntDoubleLong_BASE<TypeParam::value>;
    using Builder = typename IntDoubleLongT::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder1;
    builder1.set_Int1(-1235143)
            .set_Double1(5436.13256)
            .set_Long1(8796723423)
            .store(os);

    Builder builder2;
    builder2.set_Int1(7635894)
            .set_Double1(-675813.43124)
            .set_Long1(6584245)
            .store(os);

    Builder builder3;
    builder3.set_Int1(-132516)
            .set_Double1(-32.6543)
            .set_Long1(-234546)
            .store(os);

    IntDoubleLongT result1 = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result1, builder1);
    ASSERT_EQ(result1.get_Int1(), -1235143);
    ASSERT_EQ(result1.get_Double1(), 5436.13256);
    ASSERT_EQ(result1.get_Long1(), 8796723423);

    IntDoubleLongT result2 = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result2, builder2);
    ASSERT_EQ(result2.get_Int1(), 7635894);
    ASSERT_EQ(result2.get_Double1(), -675813.43124);
    ASSERT_EQ(result2.get_Long1(), 6584245);

    IntDoubleLongT result3 = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result3, builder3);
    ASSERT_EQ(result3.get_Int1(), -132516);
    ASSERT_EQ(result3.get_Double1(), -32.6543);
    ASSERT_EQ(result3.get_Long1(), -234546);

    ASSERT_EQ(is.pos(), 3 * IntDoubleLongT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    IntDoubleLongT new_result1 = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    IntDoubleLongT new_result2 = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    IntDoubleLongT new_result3 = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(is.pos(), 6 * IntDoubleLongT::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(IntDoubleLongFixture, SingleWithOverlap)
{
    using IntDoubleLongT = simple::IntDoubleLong_BASE<TypeParam::value>;
    using Builder = typename IntDoubleLongT::Builder;

    const size_t size = 16;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    Builder builder;
    builder.set_Int1(56192435)
            .set_Double1(543.1432435)
            .set_Long1(-12359176)
            .store(os);

    IntDoubleLongT result = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result, builder);
    ASSERT_EQ(result.get_Int1(), 56192435);
    ASSERT_EQ(result.get_Double1(), 543.1432435);
    ASSERT_EQ(result.get_Long1(), -12359176);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    IntDoubleLongT new_result = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(IntDoubleLongFixture, CoupleWithOverlap)
{
    using IntDoubleLongT = simple::IntDoubleLong_BASE<TypeParam::value>;
    using Builder = typename IntDoubleLongT::Builder;

    const size_t size = 36;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    Builder builder1;
    builder1.set_Int1(-296958)
            .set_Double1(-325.1234516)
            .set_Long1(14543167)
            .store(os);

    Builder builder2;
    builder2.set_Int1(8196143)
            .set_Double1(4534.75907)
            .set_Long1(4351)
            .store(os);

    Builder builder3;
    builder3.set_Int1(1235)
            .set_Double1(65432147.965847)
            .set_Long1(-14362)
            .store(os);

    IntDoubleLongT result1 = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result1, builder1);
    ASSERT_EQ(result1.get_Int1(), -296958);
    ASSERT_EQ(result1.get_Double1(), -325.1234516);
    ASSERT_EQ(result1.get_Long1(), 14543167);

    IntDoubleLongT result2 = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result2, builder2);
    ASSERT_EQ(result2.get_Int1(), 8196143);
    ASSERT_EQ(result2.get_Double1(), 4534.75907);
    ASSERT_EQ(result2.get_Long1(), 4351);

    IntDoubleLongT result3 = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result3, builder3);
    ASSERT_EQ(result3.get_Int1(), 1235);
    ASSERT_EQ(result3.get_Double1(), 65432147.965847);
    ASSERT_EQ(result3.get_Long1(), -14362);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    IntDoubleLongT new_result1 = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    IntDoubleLongT new_result2 = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    IntDoubleLongT new_result3 = IntDoubleLongT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(IntDoubleLongFixture, Random)
{
    using IntDoubleLongT = simple::IntDoubleLong_BASE<TypeParam::value>;
    using Builder = typename IntDoubleLongT::Builder;

    const size_t size = 20;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 1000; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 300; ++i) {
        Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder.store(os);

        IntDoubleLongT result = IntDoubleLongT::fetch(is);
        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        IntDoubleLongT new_result = IntDoubleLongT::fetch(is);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(IntDoubleLongFixture, Array)
{
    using IntDoubleLongT = simple::IntDoubleLong_BASE<TypeParam::value>;

    const size_t size = 16;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 8000; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 256; ++i) {
        using ArrayBuilder = array<IntDoubleLongT>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<IntDoubleLongT> elems;
        {
            array<IntDoubleLongT> result = array<IntDoubleLongT>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            array<IntDoubleLongT> new_result = array<IntDoubleLongT>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}

TYPED_TEST(IntDoubleLongFixture, ArrayMagic)
{
    using IntDoubleLongT = simple::IntDoubleLong_BASE<TypeParam::value>;

    const size_t size = 16;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 8000; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 256; ++i) {
        using ArrayBuilder = Array<IntDoubleLongT>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<IntDoubleLongT> elems;
        {
            Array<IntDoubleLongT> result = Array<IntDoubleLongT>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            Array<IntDoubleLongT> new_result = Array<IntDoubleLongT>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}

TEST(IntDoubleLong, WrongMagic)
{
    Allocator::startScope();
    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    String::Builder builder;
    builder.store(os);

    ASSERT_THROW(simple::IntDoubleLong::fetch(is), TLException);
    Allocator::endScope();
}
