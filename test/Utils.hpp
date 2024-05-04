#pragma once

#include <gtest/gtest.h>

#include "prelude/DataStream.hpp"

#include <random>



extern "C" {
void __ubsan_on_report()
{
    FAIL() << "Encountered an undefined behavior sanitizer error";
}
void __asan_on_error()
{
    FAIL() << "Encountered an address sanitizer error";
}
void __tsan_on_report()
{
    FAIL() << "Encountered a thread sanitizer error";
}
}    // extern "C"



DataStream::Block createBlock(size_t size)
{
    auto data = std::shared_ptr<uint8_t[]>(new uint8_t[size]);
    return {data, size};
}



template <typename T>
void testCopyMoveIteration(T t)
requires(!requires(size_t i) { t.size(); t[i]; })
{}

template <typename T>
void testCopyMoveIteration(T t)
requires(requires(size_t i) { t.size(); t[i]; });

// no universal ref
// check for copy/move semantics
template <typename T>
void testCopyMove(T t)
{
    T t1(t);
    ASSERT_EQ(t, t1);

    T t2;
    {
        T t3 = {};
        t3 = t;
        ASSERT_EQ(t, t3);

        t2 = t3;
    }
    ASSERT_EQ(t, t2);

    T t4(std::move(t2));
    ASSERT_EQ(t, t4);

    T t5 = t4;
    ASSERT_EQ(t5, t4);

    t5 = std::move(t4);
    ASSERT_EQ(t, t5);

    T t6 = t;
    T t7 = t;
    {
        T t8 = t;

        ASSERT_EQ(t6, t7);
        ASSERT_EQ(t6, t8);
        ASSERT_EQ(t6, t);

        t6 = std::move(t7);
        ASSERT_EQ(t6, t);
    }
    ASSERT_EQ(t, t6);

    T t9(t);
    ASSERT_EQ(t, t9);

    t9 = T {};
    T t10(t);
    t10 = t9;
    ASSERT_EQ(t10, T {});

    testCopyMoveIteration(t);
}

// no universal ref
// check for copy/move semantics
template <typename T>
void testCopyMoveIteration(T t)
requires(requires(size_t i) { t.size(); t[i]; })
{
    for (size_t i = 0; i < t.size(); ++i) {
        testCopyMove(t[i]);
    }

    for (auto curr : t) {
        testCopyMove(curr);
    }
}

//#define BASELINE
//#define INLINE
//#define PROXY_NAIVE
#define PROXY



constexpr size_t SEED = 777;

// NOLINTNEXTLINE
std::default_random_engine DEFAULT_RANDOM_ENGINE(SEED);


#ifdef BASELINE
#include "samples/BaselineIncludes.hpp"
using namespace baseline;
#elifdef INLINE
#include "samples/InlineIncludes.hpp"
using namespace opus;
using namespace opus::inl;
#elifdef PROXY_NAIVE
#include "samples/ProxyNaiveIncludes.hpp"
using namespace opus;
using namespace opus::proxy_naive;
#elifdef PROXY
#include "samples/ProxyIncludes.hpp"
using namespace opus;
using namespace opus::proxy;
#endif


//namespace statshouse {
//
//template <TLType T>
//bool operator==(const DictionaryField<T>& lhs, const DictionaryField<T>& rhs)
//{
//    return lhs.get_key() == rhs.get_key()
//           && lhs.get_value() == rhs.get_value();
//}
//
//
//
//template <TLType T>
//bool operator==(const Dictionary<T>& lhs, const Dictionary<T>& rhs)
//{
//    return lhs.get_data() == rhs.get_data();
//}
//
//
//
//bool operator==(const Metric& lhs, const Metric& rhs)
//{
//    return lhs.get_fields_mask() == rhs.get_fields_mask()
//           && lhs.get_name() == rhs.get_name()
//           && lhs.get_tags() == rhs.get_tags()
//           && lhs.get_counter() == rhs.get_counter()
//           && lhs.get_ts() == rhs.get_ts()
//           && lhs.get_value() == rhs.get_value()
//           && lhs.get_unique() == rhs.get_unique();
//}
//
//
//
//bool operator==(const AddMetricsBatchQuery& lhs, const AddMetricsBatchQuery& rhs)
//{
//    return lhs.get_fields_mask() == rhs.get_fields_mask()
//           && lhs.get_metrics() == rhs.get_metrics();
//}
