#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <random>

#define IS_SET(mask, pos) (!!((mask) & (1 << (pos))))



using magic_t = uint32_t;
using offset_t = uint32_t;
using ref_cnt_t = uint32_t;
using symbol_t = char;


constexpr offset_t NO_VALUE = -1;


template <typename P>
concept Primitive = !std::is_pointer_v<P>
                    && (sizeof(P) % sizeof(int32_t) == 0)
                    && (std::is_integral_v<P> || std::is_floating_point_v<P>);

template <typename C>
concept Struct = C::STATIC == true;

template <typename C>
concept StaticType = std::is_same_v<C, symbol_t> || Primitive<C> || Struct<C>;

template <typename C>
concept DynamicType = !Struct<C> && C::STATIC == false;

template <typename C>
concept TLType = StaticType<C> || DynamicType<C>;



struct TLException : public std::exception {
public:
    enum class TYPE {
        BAD_MAGIC,
        BAD_MASK,
        END,
    };

    explicit TLException(TYPE type)
        : m_type(type)
    {}

    TYPE m_type;
};

namespace utils {

template <uint32_t MIN_VALUE, uint32_t MAX_VALUE>
uint32_t random_mask(std::default_random_engine& engine)
{
    static std::uniform_int_distribution<uint32_t> ud(MIN_VALUE, MAX_VALUE);
    return ud(engine);
}

constexpr uint32_t commutative(uint32_t i, uint32_t j)
{
    if (j == 0) j = std::numeric_limits<uint32_t>::max();
    return (i * j + (i * i) * (j * j) + (i * i * i) * (j * j * j)) % std::numeric_limits<uint32_t>::max();
};

}    // namespace utils
