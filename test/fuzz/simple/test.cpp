#include "fuzztest/fuzztest.h"
#include <gtest/gtest.h>

#include "test/fuzz/FuzzBlockStream.hpp"

#include "test/fuzz/simple/CompoundCTS.hpp"
#include "test/fuzz/simple/CompoundRTS.hpp"
#include "test/fuzz/simple/IntDouble.hpp"
#include "test/fuzz/simple/IntDoubleLong.hpp"
#include "test/fuzz/simple/Picture.hpp"
#include "test/fuzz/simple/Point.hpp"
#include "test/fuzz/simple/Rectangle.hpp"
#include "test/fuzz/simple/StringIntLong.hpp"

#include "test/fuzz/ArrayPrimitive.hpp"
#include "test/fuzz/String.hpp"



auto SimpleDomain()
{
    return fuzztest::VariantOf(
            fuzz_details::domainString(),
            fuzz_details::domainArrayArrayString<true>(),
            fuzz_details::domainArrayArrayString<false>(),

            fuzz_details::domainArrayPrimitive<int>(),
            fuzz_details::domainArrayArrayPrimitive<int, true>(),
            fuzz_details::domainArrayArrayPrimitive<int, false>(),
            fuzz_details::domainArrayPrimitive<float>(),
            fuzz_details::domainArrayArrayPrimitive<float, true>(),
            fuzz_details::domainArrayArrayPrimitive<float, false>(),
            fuzz_details::domainArrayPrimitive<long>(),
            fuzz_details::domainArrayArrayPrimitive<long, true>(),
            fuzz_details::domainArrayArrayPrimitive<long, false>(),
            fuzz_details::domainArrayPrimitive<double>(),
            fuzz_details::domainArrayArrayPrimitive<double, true>(),
            fuzz_details::domainArrayArrayPrimitive<double, false>(),

            fuzz_details::domainIntDouble(),
            fuzz_details::domainArrayIntDouble<true>(),
            fuzz_details::domainArrayIntDouble<false>(),

            fuzz_details::domainIntDoubleLong(),
            fuzz_details::domainArrayIntDoubleLong<true>(),
            fuzz_details::domainArrayIntDoubleLong<false>(),

            fuzz_details::domainCompoundCTS(),
            fuzz_details::domainArrayCompoundCTS<true>(),
            fuzz_details::domainArrayCompoundCTS<false>(),

            fuzz_details::domainStringIntLong(),
            fuzz_details::domainArrayStringIntLong<true>(),
            fuzz_details::domainArrayStringIntLong<false>(),

            fuzz_details::domainCompoundRTS(),
            fuzz_details::domainArrayCompoundRTS<true>(),
            fuzz_details::domainArrayCompoundRTS<false>(),

            fuzz_details::domainPoint(),
            fuzz_details::domainArrayPoint<true>(),
            fuzz_details::domainArrayPoint<false>(),

            fuzz_details::domainPoint_s<0>(),
            fuzz_details::domainArrayPoint_s<true, 0>(),
            fuzz_details::domainArrayPoint_s<false, 0>(),

            fuzz_details::domainPoint_s<1>(),
            fuzz_details::domainArrayPoint_s<true, 1>(),
            fuzz_details::domainArrayPoint_s<false, 1>(),

            fuzz_details::domainPoint_s<2>(),
            fuzz_details::domainArrayPoint_s<true, 2>(),
            fuzz_details::domainArrayPoint_s<false, 2>(),

            fuzz_details::domainPoint_s<3>(),
            fuzz_details::domainArrayPoint_s<true, 3>(),
            fuzz_details::domainArrayPoint_s<false, 3>(),

            fuzz_details::domainPoint_s<4>(),
            fuzz_details::domainArrayPoint_s<true, 4>(),
            fuzz_details::domainArrayPoint_s<false, 4>(),

            fuzz_details::domainPoint_s<5>(),
            fuzz_details::domainArrayPoint_s<true, 5>(),
            fuzz_details::domainArrayPoint_s<false, 5>(),

            fuzz_details::domainPoint_s<6>(),
            fuzz_details::domainArrayPoint_s<true, 6>(),
            fuzz_details::domainArrayPoint_s<false, 6>(),

            fuzz_details::domainPoint_s<7>(),
            fuzz_details::domainArrayPoint_s<true, 7>(),
            fuzz_details::domainArrayPoint_s<false, 7>(),

            fuzz_details::domainRectangle(),
            fuzz_details::domainArrayRectangle<true>(),
            fuzz_details::domainArrayRectangle<false>(),

            fuzz_details::domainRectangle_s<0>(),
            fuzz_details::domainArrayRectangle_s<true, 0>(),
            fuzz_details::domainArrayRectangle_s<false, 0>(),

            fuzz_details::domainRectangle_s<1>(),
            fuzz_details::domainArrayRectangle_s<true, 1>(),
            fuzz_details::domainArrayRectangle_s<false, 1>(),

            fuzz_details::domainRectangle_s<2>(),
            fuzz_details::domainArrayRectangle_s<true, 2>(),
            fuzz_details::domainArrayRectangle_s<false, 2>(),

            fuzz_details::domainRectangle_s<3>(),
            fuzz_details::domainArrayRectangle_s<true, 3>(),
            fuzz_details::domainArrayRectangle_s<false, 3>(),

            fuzz_details::domainRectangle_s<4>(),
            fuzz_details::domainArrayRectangle_s<true, 4>(),
            fuzz_details::domainArrayRectangle_s<false, 4>(),

            fuzz_details::domainRectangle_s<5>(),
            fuzz_details::domainArrayRectangle_s<true, 5>(),
            fuzz_details::domainArrayRectangle_s<false, 5>(),

            fuzz_details::domainRectangle_s<6>(),
            fuzz_details::domainArrayRectangle_s<true, 6>(),
            fuzz_details::domainArrayRectangle_s<false, 6>(),

            fuzz_details::domainRectangle_s<7>(),
            fuzz_details::domainArrayRectangle_s<true, 7>(),
            fuzz_details::domainArrayRectangle_s<false, 7>(),

            fuzz_details::domainPicture(),
            fuzz_details::domainArrayPicture<true>(),
            fuzz_details::domainArrayPicture<false>()
    );
}

using SimpleVariant = std::variant<
        fuzz_details::String,
        fuzz_details::ArrayString<true>,
        fuzz_details::ArrayString<false>,

        fuzz_details::ArrayPrimitive<int>,
        fuzz_details::ArrayArrayPrimitive<int, true>,
        fuzz_details::ArrayArrayPrimitive<int, false>,
        fuzz_details::ArrayPrimitive<float>,
        fuzz_details::ArrayArrayPrimitive<float, true>,
        fuzz_details::ArrayArrayPrimitive<float, false>,
        fuzz_details::ArrayPrimitive<long>,
        fuzz_details::ArrayArrayPrimitive<long, true>,
        fuzz_details::ArrayArrayPrimitive<long, false>,
        fuzz_details::ArrayPrimitive<double>,
        fuzz_details::ArrayArrayPrimitive<double, true>,
        fuzz_details::ArrayArrayPrimitive<double, false>,

        fuzz_details::IntDouble,
        fuzz_details::ArrayIntDouble<true>,
        fuzz_details::ArrayIntDouble<false>,

        fuzz_details::IntDoubleLong,
        fuzz_details::ArrayIntDoubleLong<true>,
        fuzz_details::ArrayIntDoubleLong<false>,

        fuzz_details::CompoundCTS,
        fuzz_details::ArrayCompoundCTS<true>,
        fuzz_details::ArrayCompoundCTS<false>,

        fuzz_details::StringIntLong,
        fuzz_details::ArrayStringIntLong<true>,
        fuzz_details::ArrayStringIntLong<false>,

        fuzz_details::CompoundRTS,
        fuzz_details::ArrayCompoundRTS<true>,
        fuzz_details::ArrayCompoundRTS<false>,

        fuzz_details::Point,
        fuzz_details::ArrayPoint<true>,
        fuzz_details::ArrayPoint<false>,

        fuzz_details::Point_s<0>,
        fuzz_details::ArrayPoint_s<true, 0>,
        fuzz_details::ArrayPoint_s<false, 0>,

        fuzz_details::Point_s<1>,
        fuzz_details::ArrayPoint_s<true, 1>,
        fuzz_details::ArrayPoint_s<false, 1>,

        fuzz_details::Point_s<2>,
        fuzz_details::ArrayPoint_s<true, 2>,
        fuzz_details::ArrayPoint_s<false, 2>,

        fuzz_details::Point_s<3>,
        fuzz_details::ArrayPoint_s<true, 3>,
        fuzz_details::ArrayPoint_s<false, 3>,

        fuzz_details::Point_s<4>,
        fuzz_details::ArrayPoint_s<true, 4>,
        fuzz_details::ArrayPoint_s<false, 4>,

        fuzz_details::Point_s<5>,
        fuzz_details::ArrayPoint_s<true, 5>,
        fuzz_details::ArrayPoint_s<false, 5>,

        fuzz_details::Point_s<6>,
        fuzz_details::ArrayPoint_s<true, 6>,
        fuzz_details::ArrayPoint_s<false, 6>,

        fuzz_details::Point_s<7>,
        fuzz_details::ArrayPoint_s<true, 7>,
        fuzz_details::ArrayPoint_s<false, 7>,

        fuzz_details::Rectangle,
        fuzz_details::ArrayRectangle<true>,
        fuzz_details::ArrayRectangle<false>,

        fuzz_details::Rectangle_s<0>,
        fuzz_details::ArrayRectangle_s<true, 0>,
        fuzz_details::ArrayRectangle_s<false, 0>,

        fuzz_details::Rectangle_s<1>,
        fuzz_details::ArrayRectangle_s<true, 1>,
        fuzz_details::ArrayRectangle_s<false, 1>,

        fuzz_details::Rectangle_s<2>,
        fuzz_details::ArrayRectangle_s<true, 2>,
        fuzz_details::ArrayRectangle_s<false, 2>,

        fuzz_details::Rectangle_s<3>,
        fuzz_details::ArrayRectangle_s<true, 3>,
        fuzz_details::ArrayRectangle_s<false, 3>,

        fuzz_details::Rectangle_s<4>,
        fuzz_details::ArrayRectangle_s<true, 4>,
        fuzz_details::ArrayRectangle_s<false, 4>,

        fuzz_details::Rectangle_s<5>,
        fuzz_details::ArrayRectangle_s<true, 5>,
        fuzz_details::ArrayRectangle_s<false, 5>,

        fuzz_details::Rectangle_s<6>,
        fuzz_details::ArrayRectangle_s<true, 6>,
        fuzz_details::ArrayRectangle_s<false, 6>,

        fuzz_details::Rectangle_s<7>,
        fuzz_details::ArrayRectangle_s<true, 7>,
        fuzz_details::ArrayRectangle_s<false, 7>,

        fuzz_details::Picture,
        fuzz_details::ArrayPicture<true>,
        fuzz_details::ArrayPicture<false>
        >;



class SimpleFuzzBlockStream : public FuzzBlockStream {
public:
    SimpleFuzzBlockStream()
        : FuzzBlockStream()
    {
    }

    void storeFetch(bool magic, const SimpleVariant& input)
    {
        checkBlockStreamSize();
        std::visit([&](auto&& arg) {
            if (magic) {
                doStoreFetch<true>(arg);
            } else {
                doStoreFetch<true>(arg);
            }
        },
                   input);
    }

    template <bool MAGIC, typename ARG>
    void doStoreFetch(ARG&& arg)
    requires(!requires { std::decay_t<ARG>::MAX_MASK_VALUE; })
    {
        auto builder = arg.template toBuilder<true>();
        using TL_TYPE = std::decay_t<decltype(builder)>::TYPE;

        builder.store(os);

        auto result = TL_TYPE::fetch(is);
        ASSERT_EQ(result, builder);
        ASSERT_TRUE(os.pos() % 4 == 0);
        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os);
        auto new_result = TL_TYPE::fetch(is);
        ASSERT_EQ(result, new_result);
        ASSERT_TRUE(os.pos() % 4 == 0);
        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }

    template <bool MAGIC, typename ARG>
    void doStoreFetch(ARG&& arg)
    requires(requires { std::decay_t<ARG>::MAX_MASK_VALUE; })
    {
        auto builder = arg.template toBuilder<true>();
        using TL_TYPE = std::decay_t<decltype(builder)>::TYPE;
        Nat mask = random_mask<std::decay_t<ARG>::MAX_MASK_VALUE>(DEFAULT_RANDOM_ENGINE);

        builder.store(os, mask);

        auto result = TL_TYPE::fetch(is, mask);
        ASSERT_EQ(result, builder);
        ASSERT_TRUE(os.pos() % 4 == 0);
        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());

        testCopyMove(result);

        result.store(os, mask);
        auto new_result = TL_TYPE::fetch(is, mask);
        ASSERT_EQ(result, new_result);
        ASSERT_TRUE(os.pos() % 4 == 0);
        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }
};



FUZZ_TEST_F(SimpleFuzzBlockStream, storeFetch).WithDomains(fuzztest::Arbitrary<bool>(), SimpleDomain());
