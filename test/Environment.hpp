#pragma once

#include <random>
#include <cstdint>

//#define BASELINE
//#define INLINE
#define PROXY_NAIVE
//#define PROXY

constexpr size_t SEED = 777;
std::default_random_engine DEFAULT_RANDOM_ENGINE(SEED); // NOLINT

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