#pragma once

#include "test/Utils.hpp"

static_assert(String::MAGIC != 0);



template <typename T>
class StringFixture : public testing::Test {};

using StringMagic = ::testing::Types<std::true_type, std::false_type>;
TYPED_TEST_SUITE(StringFixture, StringMagic);



TYPED_TEST(StringFixture, Empty)
{
    using StringT = ArrayBase<symbol_t, TypeParam::value>;
    using Builder = typename StringT::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder;
    builder.setString("");

    builder.store(os);
    builder.store(os);
    builder.store(os);

    StringT result1 = StringT::fetch(is);
    ASSERT_EQ(result1, builder);
    ASSERT_EQ(result1.size(), 0);

    StringT result2 = StringT::fetch(is);
    ASSERT_EQ(result2, builder);
    ASSERT_EQ(result2.size(), 0);

    StringT result3 = StringT::fetch(is);
    ASSERT_EQ(result3, builder);
    ASSERT_EQ(result3.size(), 0);

    ASSERT_EQ(os.pos(), 3 * (TypeParam::value * Magic::SIZEOF + sizeof(uint32_t)));
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    StringT new_result1 = StringT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    StringT new_result2 = StringT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    StringT new_result3 = StringT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), 6 * (TypeParam::value * Magic::SIZEOF + sizeof(uint32_t)));
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(StringFixture, Single)
{
    using StringT = ArrayBase<symbol_t, TypeParam::value>;
    using Builder = typename StringT::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    const std::string str = "Hello, world!";

    Builder builder;
    builder.setString(str)
            .store(os);

    StringT result = StringT::fetch(is);
    ASSERT_EQ(result, builder);
    ASSERT_EQ(result, str);

    ASSERT_EQ(os.pos(), TypeParam::value * Magic::SIZEOF + 16);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    StringT new_result = StringT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), 2 * (TypeParam::value * Magic::SIZEOF + 16));
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(StringFixture, Couple)
{
    using StringT = ArrayBase<symbol_t, TypeParam::value>;
    using Builder = typename StringT::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    const std::string str1 = "London is the capital of Great Britain";
    const std::string str2 = "Apple a day keeps a doctor away";
    const std::string str3 = "Where?";
    const std::string str4 = "Here";

    Builder builder1;
    builder1.setString(str1)
            .store(os);

    Builder builder2;
    builder2.setString(str2)
            .store(os);

    Builder builder3;
    builder3.setString(str3)
            .store(os);

    Builder builder4;
    builder4.setString(str4)
            .store(os);

    StringT result1 = StringT::fetch(is);
    ASSERT_EQ(result1, builder1);
    ASSERT_EQ(result1, str1);

    StringT result2 = StringT::fetch(is);
    ASSERT_EQ(result2, builder2);
    ASSERT_EQ(result2, str2);

    StringT result3 = StringT::fetch(is);
    ASSERT_EQ(result3, builder3);
    ASSERT_EQ(result3, str3);

    StringT result4 = StringT::fetch(is);
    ASSERT_EQ(result4, builder4);
    ASSERT_EQ(result4, str4);

    ASSERT_EQ(os.pos(), 4 * TypeParam::value * Magic::SIZEOF + 88);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);
    testCopyMove(result4);

    result1.store(os);
    StringT new_result1 = StringT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    StringT new_result2 = StringT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    StringT new_result3 = StringT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    result4.store(os);
    StringT new_result4 = StringT::fetch(is);
    ASSERT_EQ(result4, new_result4);

    ASSERT_EQ(os.pos(), 2 * (4 * TypeParam::value * Magic::SIZEOF + 88));
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(StringFixture, SingleWithOverlap)
{
    using StringT = ArrayBase<symbol_t, TypeParam::value>;
    using Builder = typename StringT::Builder;

    const size_t size = 16;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    const std::string str = "qwertyuiopasdfghjklzxcvbnm";

    Builder builder;
    builder.setString(str)
            .store(os);

    StringT result = StringT::fetch(is);
    ASSERT_EQ(result, builder);
    ASSERT_EQ(result, str);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    StringT new_result = StringT::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(StringFixture, CoupleWithOverlap)
{
    using StringT = ArrayBase<symbol_t, TypeParam::value>;
    using Builder = typename StringT::Builder;

    const size_t size = 32;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    const std::string str1 = "Amaterasu...!";
    const std::string str2 = "thethethethethethethe";
    const std::string str3 = "ajdkslg;mad";

    Builder builder1;
    builder1.setString(str1)
            .store(os);

    Builder builder2;
    builder2.setString(str2)
            .store(os);

    Builder builder3;
    builder3.setString(str3)
            .store(os);

    StringT result1 = StringT::fetch(is);
    ASSERT_EQ(result1, builder1);
    ASSERT_EQ(result1, str1);

    StringT result2 = StringT::fetch(is);
    ASSERT_EQ(result2, builder2);
    ASSERT_EQ(result2, str2);

    StringT result3 = StringT::fetch(is);
    ASSERT_EQ(result3, builder3);
    ASSERT_EQ(result3, str3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    StringT new_result1 = StringT::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    StringT new_result2 = StringT::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    StringT new_result3 = StringT::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(StringFixture, Padding)
{
    using StringT = ArrayBase<symbol_t, TypeParam::value>;
    using Builder = typename StringT::Builder;

    const size_t size = TypeParam::value ? 32 : 16;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};

    Builder().setString("0").store(os);
    Builder().setString("1").store(os);
    Builder().setString("2").store(os);
    Builder().setString("3").store(os);

    ASSERT_THROW(Builder().setString("4").store(os), TLException);
}

TYPED_TEST(StringFixture, Random)
{
    using StringT = ArrayBase<symbol_t, TypeParam::value>;
    using Builder = typename StringT::Builder;

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

    for (size_t i = 0; i < 900; ++i) {
        Builder builder = Builder::template random<1024>(DEFAULT_RANDOM_ENGINE);
        builder.store(os);

        StringT result = StringT::fetch(is);
        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        StringT new_result = StringT::fetch(is);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(StringFixture, Array)
{
    using StringT = ArrayBase<symbol_t, TypeParam::value>;

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
        using ArrayBuilder = array<StringT>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10, 256>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<StringT> elems;
        {
            array<StringT> result = array<StringT>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            array<StringT> new_result = array<StringT>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}

TEST(String, WrongMagic)
{
    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Magic wrong_magic = ~String::MAGIC;
    wrong_magic.store(os);

    ASSERT_THROW(String::fetch(is), TLException);
}
