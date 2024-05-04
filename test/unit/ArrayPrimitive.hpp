#pragma once

#include "test/Utils.hpp"



static_assert(Array<Int>::MAGIC != 0);
static_assert(Array<Long>::MAGIC != 0);
static_assert(Array<Float>::MAGIC != 0);
static_assert(Array<Double>::MAGIC != 0);



template <typename T>
class ArrayPrimitive : public testing::Test {};

using Primitives = ::testing::Types<int, float, long, double>;
TYPED_TEST_SUITE(ArrayPrimitive, Primitives);



TYPED_TEST(ArrayPrimitive, MagicEmpty)
{
    using ArrayP = Array<Scalar<TypeParam>>;
    using Builder = typename ArrayP::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder;

    builder.store(os);
    builder.store(os);
    builder.store(os);

    ArrayP result1 = ArrayP::fetch(is);
    ASSERT_EQ(result1, builder);
    ASSERT_EQ(result1.size(), 0);

    ArrayP result2 = ArrayP::fetch(is);
    ASSERT_EQ(result2, builder);
    ASSERT_EQ(result2.size(), 0);

    ArrayP result3 = ArrayP::fetch(is);
    ASSERT_EQ(result3, builder);
    ASSERT_EQ(result3.size(), 0);

    ASSERT_EQ(os.pos(), 3 * (Magic::SIZEOF + sizeof(uint32_t)));
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    ArrayP new_result1 = ArrayP::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    ArrayP new_result2 = ArrayP::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    ArrayP new_result3 = ArrayP::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), 6 * (Magic::SIZEOF + sizeof(uint32_t)));
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(ArrayPrimitive, Empty)
{
    using arrayP = array<Scalar<TypeParam>>;
    using Builder = typename arrayP::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    Builder builder;

    builder.store(os);
    builder.store(os);
    builder.store(os);

    arrayP result1 = arrayP::fetch(is);
    ASSERT_EQ(result1, builder);
    ASSERT_EQ(result1.size(), 0);

    arrayP result2 = arrayP::fetch(is);
    ASSERT_EQ(result2, builder);
    ASSERT_EQ(result2.size(), 0);

    arrayP result3 = arrayP::fetch(is);
    ASSERT_EQ(result3, builder);
    ASSERT_EQ(result3.size(), 0);

    ASSERT_EQ(os.pos(), 3 *sizeof(uint32_t));
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);

    result1.store(os);
    arrayP new_result1 = arrayP::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    arrayP new_result2 = arrayP::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    arrayP new_result3 = arrayP::fetch(is);
    ASSERT_EQ(result3, new_result3);

    ASSERT_EQ(os.pos(), 6 *sizeof(uint32_t));
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(ArrayPrimitive, Single)
{
    using ArrayP = Array<Scalar<TypeParam>>;
    using Builder = typename ArrayP::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    const std::vector<TypeParam> vec = {42, 21, 1337, 100500, 777, 153461, 73568, 9583562};
    Builder builder;
    builder.setArray(vec)
            .store(os);

    ArrayP result = ArrayP::fetch(is);
    ASSERT_EQ(result, builder);
    ASSERT_EQ(result, vec);

    ASSERT_EQ(os.pos(), Magic::SIZEOF + sizeof(uint32_t) + result.size() * Scalar<TypeParam>::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    ArrayP new_result = ArrayP::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), 2 * (Magic::SIZEOF + sizeof(uint32_t) + result.size() * Scalar<TypeParam>::SIZEOF));
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(ArrayPrimitive, Couple)
{
    using ArrayP = Array<Scalar<TypeParam>>;
    using Builder = typename ArrayP::Builder;

    const size_t size = 1024;
    DataStream::Block block = createBlock(size);
    OutputStream os = {block};
    InputStream is = {block};

    const std::vector<TypeParam> vec1 = {12, 6735684, 5780, 2456, 56385876, 69};
    const std::vector<TypeParam> vec2 = {-98124, -4132552};
    const std::vector<TypeParam> vec3 = {94382, 6576437, 4678, 143541, 0, 0, 0};
    const std::vector<TypeParam> vec4 = {78519, 5183, -134, 513451, 52462, 6246, 756874, 84, 12341, 52462, 74567};

    Builder builder1;
    builder1.setArray(vec1)
            .store(os);

    Builder builder2;
    builder2.setArray(vec2)
            .store(os);

    Builder builder3;
    builder3.setArray(vec3)
            .store(os);

    Builder builder4;
    builder4.setArray(vec4)
            .store(os);

    ArrayP result1 = ArrayP::fetch(is);
    ASSERT_EQ(result1, builder1);
    ASSERT_EQ(result1, vec1);

    ArrayP result2 = ArrayP::fetch(is);
    ASSERT_EQ(result2, builder2);
    ASSERT_EQ(result2, vec2);

    ArrayP result3 = ArrayP::fetch(is);
    ASSERT_EQ(result3, builder3);
    ASSERT_EQ(result3, vec3);

    ArrayP result4 = ArrayP::fetch(is);
    ASSERT_EQ(result4, builder4);
    ASSERT_EQ(result4, vec4);

    ASSERT_EQ(os.pos(), 4 * (Magic::SIZEOF + sizeof(uint32_t))
                                + result1.size() * Scalar<TypeParam>::SIZEOF
                                + result2.size() * Scalar<TypeParam>::SIZEOF
                                + result3.size() * Scalar<TypeParam>::SIZEOF
                                + result4.size() * Scalar<TypeParam>::SIZEOF);
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);
    testCopyMove(result4);

    result1.store(os);
    ArrayP new_result1 = ArrayP::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    ArrayP new_result2 = ArrayP::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    ArrayP new_result3 = ArrayP::fetch(is);
    ASSERT_EQ(result3, new_result3);

    result4.store(os);
    ArrayP new_result4 = ArrayP::fetch(is);
    ASSERT_EQ(result4, new_result4);

    ASSERT_EQ(os.pos(), 2 * (4 * (Magic::SIZEOF + sizeof(uint32_t)) + result1.size() * Scalar<TypeParam>::SIZEOF + result2.size() * Scalar<TypeParam>::SIZEOF + result3.size() * Scalar<TypeParam>::SIZEOF + result4.size() * Scalar<TypeParam>::SIZEOF));
    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(ArrayPrimitive, SingleWithOverlap)
{
    using ArrayP = Array<Scalar<TypeParam>>;
    using Builder = typename ArrayP::Builder;

    const size_t size = 24;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    const std::vector<TypeParam> vec = {5135, 26, 3657, 7456, 236};

    Builder builder;
    builder.setArray(vec)
            .store(os);

    ArrayP result = ArrayP::fetch(is);
    ASSERT_EQ(result, builder);
    ASSERT_EQ(result, vec);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result);

    result.store(os);
    ArrayP new_result = ArrayP::fetch(is);
    ASSERT_EQ(result, new_result);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(ArrayPrimitive, CoupleWithOverlap)
{
    using ArrayP = Array<Scalar<TypeParam>>;
    using Builder = typename ArrayP::Builder;

    const size_t size = 128;
    DataStream::Block block1 = createBlock(size);
    DataStream::Block block2 = createBlock(size);
    DataStream::Block block3 = createBlock(size);
    DataStream::Block block4 = createBlock(size);
    OutputStream os = {block1, block2, block3, block4};
    InputStream is = {block1, block2, block3, block4};

    const std::vector<TypeParam> vec1 = {5146, 64579, 96, 24361, 6356, 2636};
    const std::vector<TypeParam> vec2 = {0, -2, -51345};
    const std::vector<TypeParam> vec3 = {6256, 63134, 54315, 65387, 98793, 243562};
    const std::vector<TypeParam> vec4 = {543251, 6245763, 1435, 523, 5, 234, 1324, 5246, 847};

    Builder builder1;
    builder1.setArray(vec1)
            .store(os);

    Builder builder2;
    builder2.setArray(vec2)
            .store(os);

    Builder builder3;
    builder3.setArray(vec3)
            .store(os);

    Builder builder4;
    builder4.setArray(vec4)
            .store(os);

    ArrayP result1 = ArrayP::fetch(is);
    ASSERT_EQ(result1, builder1);
    ASSERT_EQ(result1, vec1);

    ArrayP result2 = ArrayP::fetch(is);
    ASSERT_EQ(result2, builder2);
    ASSERT_EQ(result2, vec2);

    ArrayP result3 = ArrayP::fetch(is);
    ASSERT_EQ(result3, builder3);
    ASSERT_EQ(result3, vec3);

    ArrayP result4 = ArrayP::fetch(is);
    ASSERT_EQ(result4, builder4);
    ASSERT_EQ(result4, vec4);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

    testCopyMove(result1);
    testCopyMove(result2);
    testCopyMove(result3);
    testCopyMove(result4);

    result1.store(os);
    ArrayP new_result1 = ArrayP::fetch(is);
    ASSERT_EQ(result1, new_result1);

    result2.store(os);
    ArrayP new_result2 = ArrayP::fetch(is);
    ASSERT_EQ(result2, new_result2);

    result3.store(os);
    ArrayP new_result3 = ArrayP::fetch(is);
    ASSERT_EQ(result3, new_result3);

    result4.store(os);
    ArrayP new_result4 = ArrayP::fetch(is);
    ASSERT_EQ(result4, new_result4);

    ASSERT_EQ(os.pos(), is.pos());
    ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
}

TYPED_TEST(ArrayPrimitive, RandomMagic)
{
    using ArrayP = Array<Scalar<TypeParam>>;
    using Builder = typename ArrayP::Builder;

    const size_t size = 128;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 2000; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 150; ++i) {
        Builder builder = Builder::template random<100>(DEFAULT_RANDOM_ENGINE);
        builder.store(os);

        ArrayP result = ArrayP::fetch(is);
        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        ArrayP new_result = ArrayP::fetch(is);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(ArrayPrimitive, Random)
{
    using arrayP = array<Scalar<TypeParam>>;
    using Builder = typename arrayP::Builder;

    const size_t size = 128;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 2000; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 150; ++i) {
        Builder builder = Builder::template random<100>(DEFAULT_RANDOM_ENGINE);
        builder.store(os);

        arrayP result = arrayP::fetch(is);
        ASSERT_EQ(result, builder);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        arrayP new_result = arrayP::fetch(is);
        ASSERT_EQ(result, new_result);

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
}

TYPED_TEST(ArrayPrimitive, Array)
{
    using ArrayP = Array<Scalar<TypeParam>>;

    const size_t size = 128;
    OutputStream os;
    InputStream is;
    std::vector<DataStream::Block> storage;

    for (size_t i = 0; i < 3000; ++i) {
        auto block = createBlock(size);
        os.add(block);
        is.add(block);
        storage.emplace_back(block);
    }

    for (size_t i = 0; i < 64; ++i) {
        using ArrayBuilder = array<ArrayP>::Builder;
        ArrayBuilder arr_builder = ArrayBuilder::template random<10, 32>(DEFAULT_RANDOM_ENGINE);
        arr_builder.store(os);

        std::vector<ArrayP> elems;
        {
            array<ArrayP> result = array<ArrayP>::fetch(is);
            ASSERT_EQ(result, arr_builder);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

            testCopyMove(result);

            elems.insert(elems.end(), result.begin(), result.end());

            result.store(os);
            array<ArrayP> new_result = array<ArrayP>::fetch(is);
            ASSERT_EQ(result, new_result);

            ASSERT_EQ(os.pos(), is.pos());
            ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        }

        testCopyMoveIteration(elems);
    }
}
