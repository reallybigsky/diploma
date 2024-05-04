#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <exception>

#define IS_SET(mask, pos) (!!((mask) & (1 << (pos))))



using magic_t = uint32_t;
using offset_t = uint32_t;
using ref_cnt_t = uint32_t;
using symbol_t = char;


constexpr offset_t NO_VALUE = -1;


template <typename P>
concept Primitive = !std::is_pointer_v<P> && (sizeof(P) % sizeof(int32_t) == 0) && (std::is_integral_v<P> || std::is_floating_point_v<P>);

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
