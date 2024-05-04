#pragma once

#include "utils/crc.hpp"

#include "prelude/Common.hpp"
#include "prelude/baseline/InputStream.hpp"
#include "prelude/baseline/OutputStream.hpp"
#include "prelude/opus/InputStream.hpp"
#include "prelude/opus/OutputStream.hpp"
#include "prelude/opus/Proxy.hpp"



template <Primitive T>
class Scalar;



using Int = Scalar<int>;
using Float = Scalar<float>;
using Long = Scalar<long>;
using Double = Scalar<double>;

using Nat = Scalar<uint32_t>;
using Magic = Scalar<magic_t>;



template <Primitive T>
class Scalar {
private:
    template <Primitive U>
    static constexpr magic_t get_type_hash() noexcept
    {
        if constexpr (std::is_same_v<int, U>) {
            return COMPILE_TIME_CRC32_STR("Int");
        } else if constexpr (std::is_same_v<long, U>) {
            return COMPILE_TIME_CRC32_STR("Long");
        } else if constexpr (std::is_same_v<float, U>) {
            return COMPILE_TIME_CRC32_STR("Float");
        } else {
            return COMPILE_TIME_CRC32_STR("Double");
        }
    }

public:
    static constexpr magic_t MAGIC = get_type_hash<T>();
    static constexpr size_t SIZEOF = sizeof(T);
    static constexpr bool STATIC = true;

    constexpr Scalar() noexcept = default;

    constexpr Scalar(const Scalar&) noexcept = default;
    constexpr Scalar(Scalar&&) noexcept = default;
    constexpr Scalar& operator=(const Scalar&) noexcept = default;
    constexpr Scalar& operator=(Scalar&&) noexcept = default;

    constexpr ~Scalar() noexcept = default;



    // NOLINTNEXTLINE
    constexpr Scalar(T t) noexcept
        : value(t)
    {}



    // NOLINTNEXTLINE
    constexpr operator T() const noexcept
    {
        return value;
    }



    constexpr bool verify() const noexcept
    {
        return true;
    }



    Scalar(const opus::Proxy& proxy, offset_t offset) noexcept
    {
        if (proxy)
            value = *(T*)(proxy.data() + offset);
    }



    static Scalar fetch(baseline::InputStream& stream)
    {
        return {stream.fetch<T>()};
    }



    void store(baseline::OutputStream& stream) const
    {
        stream.store(value);
    }



    static Scalar fetch(opus::InputStream& stream)
    {
        return {stream.fetch<T>()};
    }



    void store(opus::OutputStream& stream) const
    {
        stream.store(value);
    }



    class Builder {
    public:
        constexpr Builder() noexcept = default;

        constexpr Builder(const Builder&) noexcept = default;
        constexpr Builder(Builder&&) noexcept = default;
        constexpr Builder& operator=(const Builder&) noexcept = default;
        constexpr Builder& operator=(Builder&&) noexcept = default;

        constexpr ~Builder() noexcept = default;

        constexpr friend bool operator==(const Builder&, const Builder&) noexcept = default;

        constexpr friend bool operator==(const Builder& lhs, const Scalar<T>& rhs) noexcept
        {
            return (T)lhs == (T)rhs;
        }

        static Builder random(std::default_random_engine& engine) noexcept
        requires std::integral<T>
        {
            static std::uniform_int_distribution<T> ud(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
            return ud(engine);
        }

        static Builder random(std::default_random_engine& engine) noexcept
        requires std::floating_point<T>
        {
            static std::uniform_real_distribution<T> ud(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
            return ud(engine);
        }

        // NOLINTNEXTLINE
        constexpr Builder(T s) noexcept
            : b_value(s)
        {}

        // NOLINTNEXTLINE
        constexpr Builder(Scalar<T> s) noexcept
            : b_value(s)
        {}

        // NOLINTNEXTLINE
        constexpr operator T() const noexcept
        {
            return b_value;
        }

        // NOLINTNEXTLINE
        constexpr operator Scalar<T>() const noexcept
        {
            return b_value;
        }

        constexpr Builder& set(T t) noexcept
        {
            b_value = t;
            return *this;
        }

        void store(baseline::OutputStream& stream) const
        {
            stream.store(b_value);
        }

        void store(opus::OutputStream& stream) const
        {
            stream.store(b_value);
        }

    private:
        T b_value = {};
    };

    T value = {};
};
