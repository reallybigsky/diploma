#pragma once

#include "test/Utils.hpp"

static_assert(simple::compoundCTS::MAGIC != 0);
static_assert(simple::compoundCTS::SIZEOF == 44);
static_assert(simple::CompoundCTS::SIZEOF == 48);



template <typename T>
class CompoundCTSFixture : public testing::Test {};

using CompoundCTSMagic = ::testing::Types<std::true_type, std::false_type>;
TYPED_TEST_SUITE(CompoundCTSFixture, CompoundCTSMagic);



TYPED_TEST(CompoundCTSFixture, Single)
{
    using CompoundCTST = simple::CompoundCTS_BASE<TypeParam::value>;
    using Builder = typename CompoundCTST::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    simple::IntDouble::Builder int_double_builder;
    int_double_builder.set_Int1(23462)
            .set_Double1(869823751);

    simple::intDoubleLong::Builder int_double_long_builder;
    int_double_long_builder.set_Int1(7243651)
            .set_Double1(-416.6456)
            .set_Long1(154765);

    Builder builder;
    builder.set_IntDouble1(int_double_builder)
            .set_Long1(2435178)
            .set_IntDoubleLong1(int_double_long_builder)
            .store(os);

    CompoundCTST result = CompoundCTST::fetch(is);
    ASSERT_EQ(result, builder);
    ASSERT_EQ(result.get_IntDouble1().get_Int1(), 23462);
    ASSERT_EQ(result.get_IntDouble1().get_Double1(), 869823751);
    ASSERT_EQ(result.get_Long1(), 2435178);
    ASSERT_EQ(result.get_IntDoubleLong1().get_Int1(), 7243651);
    ASSERT_EQ(result.get_IntDoubleLong1().get_Double1(), -416.6456);
    ASSERT_EQ(result.get_IntDoubleLong1().get_Long1(), 154765);

    ASSERT_EQ(is.pos(), CompoundCTST::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    CompoundCTST new_result = CompoundCTST::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(is.pos(), 2 * CompoundCTST::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(CompoundCTSFixture, Couple)
{
    using CompoundCTST = simple::CompoundCTS_BASE<TypeParam::value>;
    using Builder = typename CompoundCTST::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    simple::IntDouble::Builder int_double_builder1;
    int_double_builder1.set_Int1(61435)
            .set_Double1(76537638.1324);

    simple::intDoubleLong::Builder int_double_long_builder1;
    int_double_long_builder1.set_Int1(1436574)
            .set_Double1(51348.13435)
            .set_Long1(3247683);

    Builder builder1;
    builder1.set_IntDouble1(int_double_builder1)
            .set_Long1(4769479)
            .set_IntDoubleLong1(int_double_long_builder1)
            .store(os);

    simple::IntDouble::Builder int_double_builder2;
    int_double_builder2.set_Int1(0)
            .set_Double1(0);

    simple::intDoubleLong::Builder int_double_long_builder2;
    int_double_long_builder2.set_Int1(0)
            .set_Double1(0)
            .set_Long1(0);

    Builder builder2;
    builder2.set_IntDouble1(int_double_builder2)
            .set_Long1(0)
            .set_IntDoubleLong1(int_double_long_builder2)
            .store(os);

    simple::IntDouble::Builder int_double_builder3;
    int_double_builder3.set_Int1(62391)
            .set_Double1(1325.1235123);

    simple::intDoubleLong::Builder int_double_long_builder3;
    int_double_long_builder3.set_Int1(236890)
            .set_Double1(90.9)
            .set_Long1(1325);

    Builder builder3;
    builder3.set_IntDouble1(int_double_builder3)
            .set_Long1(7812)
            .set_IntDoubleLong1(int_double_long_builder3)
            .store(os);

    CompoundCTST result1 = CompoundCTST::fetch(is);
    ASSERT_EQ(result1, builder1);
    ASSERT_EQ(result1.get_IntDouble1().get_Int1(), 61435);
    ASSERT_EQ(result1.get_IntDouble1().get_Double1(), 76537638.1324);
    ASSERT_EQ(result1.get_Long1(), 4769479);
    ASSERT_EQ(result1.get_IntDoubleLong1().get_Int1(), 1436574);
    ASSERT_EQ(result1.get_IntDoubleLong1().get_Double1(), 51348.13435);
    ASSERT_EQ(result1.get_IntDoubleLong1().get_Long1(), 3247683);

    CompoundCTST result2 = CompoundCTST::fetch(is);
    ASSERT_EQ(result2, builder2);
    ASSERT_EQ(result2.get_IntDouble1().get_Int1(), 0);
    ASSERT_EQ(result2.get_IntDouble1().get_Double1(), 0);
    ASSERT_EQ(result2.get_Long1(), 0);
    ASSERT_EQ(result2.get_IntDoubleLong1().get_Int1(), 0);
    ASSERT_EQ(result2.get_IntDoubleLong1().get_Double1(), 0);
    ASSERT_EQ(result2.get_IntDoubleLong1().get_Long1(), 0);

    CompoundCTST result3 = CompoundCTST::fetch(is);
    ASSERT_EQ(result3, builder3);
    ASSERT_EQ(result3.get_IntDouble1().get_Int1(), 62391);
    ASSERT_EQ(result3.get_IntDouble1().get_Double1(), 1325.1235123);
    ASSERT_EQ(result3.get_Long1(), 7812);
    ASSERT_EQ(result3.get_IntDoubleLong1().get_Int1(), 236890);
    ASSERT_EQ(result3.get_IntDoubleLong1().get_Double1(), 90.9);
    ASSERT_EQ(result3.get_IntDoubleLong1().get_Long1(), 1325);

    ASSERT_EQ(is.pos(), 3 * CompoundCTST::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    CompoundCTST new_result1 = CompoundCTST::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    CompoundCTST new_result2 = CompoundCTST::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    CompoundCTST new_result3 = CompoundCTST::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(is.pos(), 6 * CompoundCTST::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(CompoundCTSFixture, SingleWithOverlap)
{
    using CompoundCTST = simple::CompoundCTS_BASE<TypeParam::value>;
    using Builder = typename CompoundCTST::Builder;

    const size_t size = 32;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    OutputStream os = {block1, block2, block3};
    InputStream is = {block1, block2, block3};

    simple::IntDouble::Builder int_double_builder;
    int_double_builder.set_Int1(13576)
            .set_Double1(345);

    simple::intDoubleLong::Builder int_double_long_builder;
    int_double_long_builder.set_Int1(-6547)
            .set_Double1(9999999.9999)
            .set_Long1(5234951);

    Builder builder;
    builder.set_IntDouble1(int_double_builder)
            .set_Long1(235168)
            .set_IntDoubleLong1(int_double_long_builder)
            .store(os);

    CompoundCTST result = CompoundCTST::fetch(is);
    ASSERT_EQ(result, builder);
    ASSERT_EQ(result.get_IntDouble1().get_Int1(), 13576);
    ASSERT_EQ(result.get_IntDouble1().get_Double1(), 345);
    ASSERT_EQ(result.get_Long1(), 235168);
    ASSERT_EQ(result.get_IntDoubleLong1().get_Int1(), -6547);
    ASSERT_EQ(result.get_IntDoubleLong1().get_Double1(), 9999999.9999);
    ASSERT_EQ(result.get_IntDoubleLong1().get_Long1(), 5234951);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    CompoundCTST new_result = CompoundCTST::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(CompoundCTSFixture, CoupleWithOverlap)
{
    using CompoundCTST = simple::CompoundCTS_BASE<TypeParam::value>;
    using Builder = typename CompoundCTST::Builder;

    const size_t size = 72;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    simple::IntDouble::Builder int_double_builder1;
    int_double_builder1.set_Int1(14378)
            .set_Double1(7981345.1543);

    simple::intDoubleLong::Builder int_double_long_builder1;
    int_double_long_builder1.set_Int1(25489)
            .set_Double1(1325.1325)
            .set_Long1(3467);

    Builder builder1;
    builder1.set_IntDouble1(int_double_builder1)
            .set_Long1(132567)
            .set_IntDoubleLong1(int_double_long_builder1)
            .store(os);

    simple::IntDouble::Builder int_double_builder2;
    int_double_builder2.set_Int1(1456)
            .set_Double1(965.43);

    simple::intDoubleLong::Builder int_double_long_builder2;
    int_double_long_builder2.set_Int1(1436)
            .set_Double1(5484.246)
            .set_Long1(7632452);

    Builder builder2;
    builder2.set_IntDouble1(int_double_builder2)
            .set_Long1(2436547)
            .set_IntDoubleLong1(int_double_long_builder2)
            .store(os);

    simple::IntDouble::Builder int_double_builder3;
    int_double_builder3.set_Int1(0)
            .set_Double1(0);

    simple::intDoubleLong::Builder int_double_long_builder3;
    int_double_long_builder3.set_Int1(0)
            .set_Double1(0)
            .set_Long1(0);

    Builder builder3;
    builder3.set_IntDouble1(int_double_builder3)
            .set_Long1(0)
            .set_IntDoubleLong1(int_double_long_builder3)
            .store(os);

    CompoundCTST result1 = CompoundCTST::fetch(is);
    ASSERT_EQ(result1, builder1);
    ASSERT_EQ(result1.get_IntDouble1().get_Int1(), 14378);
    ASSERT_EQ(result1.get_IntDouble1().get_Double1(), 7981345.1543);
    ASSERT_EQ(result1.get_Long1(), 132567);
    ASSERT_EQ(result1.get_IntDoubleLong1().get_Int1(), 25489);
    ASSERT_EQ(result1.get_IntDoubleLong1().get_Double1(), 1325.1325);
    ASSERT_EQ(result1.get_IntDoubleLong1().get_Long1(), 3467);

    CompoundCTST result2 = CompoundCTST::fetch(is);
    ASSERT_EQ(result2, builder2);
    ASSERT_EQ(result2.get_IntDouble1().get_Int1(), 1456);
    ASSERT_EQ(result2.get_IntDouble1().get_Double1(), 965.43);
    ASSERT_EQ(result2.get_Long1(), 2436547);
    ASSERT_EQ(result2.get_IntDoubleLong1().get_Int1(), 1436);
    ASSERT_EQ(result2.get_IntDoubleLong1().get_Double1(), 5484.246);
    ASSERT_EQ(result2.get_IntDoubleLong1().get_Long1(), 7632452);

    CompoundCTST result3 = CompoundCTST::fetch(is);
    ASSERT_EQ(result3, builder3);
    ASSERT_EQ(result3.get_IntDouble1().get_Int1(), 0);
    ASSERT_EQ(result3.get_IntDouble1().get_Double1(), 0);
    ASSERT_EQ(result3.get_Long1(), 0);
    ASSERT_EQ(result3.get_IntDoubleLong1().get_Int1(), 0);
    ASSERT_EQ(result3.get_IntDoubleLong1().get_Double1(), 0);
    ASSERT_EQ(result3.get_IntDoubleLong1().get_Long1(), 0);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    CompoundCTST new_result1 = CompoundCTST::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    CompoundCTST new_result2 = CompoundCTST::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    CompoundCTST new_result3 = CompoundCTST::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(CompoundCTSFixture, Random)
{
    using CompoundCTST = simple::CompoundCTS_BASE<TypeParam::value>;
    using Builder = typename CompoundCTST::Builder;

    const size_t size = 32;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 100; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 25; ++i) {
        Builder builder = Builder::random(DEFAULT_RANDOM_ENGINE);
        builder.store(os);

        CompoundCTST result = CompoundCTST::fetch(is);
        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        CompoundCTST new_result = CompoundCTST::fetch(is);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(CompoundCTSFixture, Array)
{
    using CompoundCTST = simple::CompoundCTS_BASE<TypeParam::value>;

    const size_t size = 16;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 16000; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 256; ++i) {
        using ArrayBuilder = array<CompoundCTST>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<CompoundCTST> elems;
        {
            array<CompoundCTST> result = array<CompoundCTST>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            array<CompoundCTST> new_result = array<CompoundCTST>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}

TYPED_TEST(CompoundCTSFixture, ArrayMagic)
{
    using CompoundCTST = simple::CompoundCTS_BASE<TypeParam::value>;

    const size_t size = 16;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 16000; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 256; ++i) {
        using ArrayBuilder = Array<CompoundCTST>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<CompoundCTST> elems;
        {
            Array<CompoundCTST> result = Array<CompoundCTST>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            Array<CompoundCTST> new_result = Array<CompoundCTST>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}

TEST(CompoundCTS, WrongMagic)
{
    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    String::Builder builder;
    builder.store(os);

    ASSERT_THROW(simple::CompoundCTS::fetch(is), TLException);
}
