#pragma once

#include "prelude/opus/Proxy.hpp"

#include <cstddef>
#include <utility>



template <typename T, bool SOME, size_t TAG>
struct MAYBE;



template <typename T, size_t TAG>
struct MAYBE<T, true, TAG> {
    static constexpr bool SOME = true;

    MAYBE() noexcept = default;

    MAYBE(const MAYBE&) noexcept = default;
    MAYBE(MAYBE&&) noexcept = default;
    MAYBE& operator=(const MAYBE&) noexcept = default;
    MAYBE& operator=(MAYBE&&) noexcept = default;

    ~MAYBE() noexcept = default;



    MAYBE(const opus::Proxy& proxy, uint32_t offset) noexcept
        : value(proxy, offset)
    {}



    MAYBE(const T& other) noexcept
        : value(other)
    {}



    MAYBE(T&& other) noexcept
        : value(std::move(other))
    {}



    MAYBE& operator=(const T& other) noexcept
    {
        if (this == &other)
            return *this;

        value = other;
        return *this;
    }



    MAYBE& operator=(T&& other) noexcept
    {
        if (this == &other)
            return *this;

        value = std::move(other);
        return *this;
    }



    // NOLINTNEXTLINE
    operator const T&() const noexcept
    {
        return value;
    }



    T value = {};
};



template <typename T, size_t TAG>
struct MAYBE<T, false, TAG> {
    static constexpr bool SOME = false;

    MAYBE() noexcept = default;

    MAYBE(const MAYBE&) noexcept = default;
    MAYBE(MAYBE&&) noexcept = default;
    MAYBE& operator=(const MAYBE&) noexcept = default;
    MAYBE& operator=(MAYBE&&) noexcept = default;

    MAYBE(const T&) noexcept {}
    MAYBE(T&&) noexcept {}

    MAYBE(const opus::Proxy&, uint32_t) noexcept {}
};
