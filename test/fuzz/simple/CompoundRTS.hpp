#pragma once

#include "fuzztest/fuzztest.h"

#include "test/Utils.hpp"

#include "test/fuzz/simple/CompoundCTS.hpp"
#include "test/fuzz/simple/IntDouble.hpp"
#include "test/fuzz/simple/IntDoubleLong.hpp"
#include "test/fuzz/simple/StringIntLong.hpp"

#include "test/fuzz/ArrayPrimitive.hpp"
#include "test/fuzz/String.hpp"



namespace fuzz_details {

struct CompoundRTS {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        return typename ::simple::CompoundRTS_BASE<BOXED>::Builder {}
                .set_Array1(Array1.toBuilder<true>())
                .set_Int1(Int1)
                .set_Long1(Long1)
                .set_String1(String1.toBuilder<true>())
                .set_IntDouble1(IntDouble1.toBuilder<true>())
                .set_StringIntLong1(StringIntLong1.toBuilder<true>())
                .set_Double1(Double1)
                .set_Double2(Double2)
                .set_Double3(Double3)
                .set_Array2(Array2.toBuilder<true>())
                .set_CompoundCTS1(CompoundCTS1.toBuilder<true>())
                .set_Float1(Float1)
                .set_IntDouble2(IntDouble2.toBuilder<true>())
                .set_IntDouble3(IntDouble3.toBuilder<true>());
    }

    ArrayPrimitive<int> Array1;
    int Int1;
    long Long1;
    String String1;
    IntDouble IntDouble1;
    StringIntLong StringIntLong1;
    double Double1;
    double Double2;
    double Double3;
    ArrayIntDoubleLong<true> Array2;
    CompoundCTS CompoundCTS1;
    float Float1;
    IntDouble IntDouble2;
    IntDouble IntDouble3;
};

auto domainCompoundRTS()
{
    return fuzztest::StructOf<CompoundRTS>(domainArrayPrimitive<int>(),
                                           fuzztest::Arbitrary<int>(),
                                           fuzztest::Arbitrary<long>(),
                                           domainString(),
                                           domainIntDouble(),
                                           domainStringIntLong(),
                                           fuzztest::Finite<double>(),
                                           fuzztest::Finite<double>(),
                                           fuzztest::Finite<double>(),
                                           domainArrayIntDoubleLong<true>(),
                                           domainCompoundCTS(),
                                           fuzztest::Finite<float>(),
                                           domainIntDouble(),
                                           domainIntDouble());
}



template <bool INNER_BOXED>
struct ArrayCompoundRTS {

    template <bool BOXED>
    auto toBuilder() const noexcept
    {
        std::vector<typename simple::CompoundRTS_BASE<INNER_BOXED>::Builder> tmp;
        tmp.reserve(arr.size());
        std::ranges::for_each(arr, [&](auto&& it) { tmp.emplace_back(it.template toBuilder<INNER_BOXED>()); });

        return typename ::ArrayBase<BOXED, simple::CompoundRTS_BASE<INNER_BOXED>>::Builder {}
                .setArray(tmp);
    }

    std::vector<CompoundRTS> arr;
};

template <bool INNER_BOXED>
auto domainArrayCompoundRTS()
{
    return fuzztest::StructOf<ArrayCompoundRTS<INNER_BOXED>>(fuzztest::VectorOf(domainCompoundRTS()).WithMaxSize(16));
};

}    // namespace fuzz_details
