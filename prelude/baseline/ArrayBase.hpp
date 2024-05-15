#pragma once

#include "prelude/Common.hpp"
#include "prelude/Scalar.hpp"
#include "prelude/baseline/Allocator.hpp"
#include "prelude/baseline/InputStream.hpp"
#include "prelude/baseline/OutputStream.hpp"

#include <memory>
#include <optional>
#include <random>
#include <string>
#include <type_traits>



namespace baseline {

template <bool BOXED, TLType T>
class ArrayBase;



template <bool BOXED>
class ArrayBase<BOXED, symbol_t> {
public:
    static constexpr Magic MAGIC = 106168542;
    static constexpr bool STATIC = false;

    ArrayBase() noexcept = default;

    ArrayBase(const ArrayBase&) noexcept = default;
    ArrayBase(ArrayBase&&) noexcept = default;
    ArrayBase& operator=(const ArrayBase&) noexcept = default;
    ArrayBase& operator=(ArrayBase&&) noexcept = default;

    ~ArrayBase() noexcept = default;



    size_t size() const noexcept
    {
        return m_data.size();
    }



    symbol_t operator[](size_t idx) const noexcept
    {
        return m_data[idx];
    }



    auto begin() const noexcept
    {
        return m_data.begin();
    }



    auto end() const noexcept
    {
        return m_data.end();
    }



    static ArrayBase fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }

        auto data = stream.fetchString();
        return {std::move(data)};
    }



    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }

        stream.storeString(m_data.data(), m_data.size());
    }



    class Builder {
    public:
        using TYPE = ArrayBase;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const ArrayBase<RHS_BOXED, symbol_t>& rhs) noexcept
        {
            if (lhs.b_str.size() != rhs.size())
                return false;

            for (size_t i = 0; i < rhs.size(); ++i) {
                if (lhs.b_str[i] != rhs[i])
                    return false;
            }

            return true;
        }

        template <size_t MAX_SIZE>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            static constexpr char ALPHANUM[] = "0123456789"
                                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                               "abcdefghijklmnopqrstuvwxyz";

            static std::uniform_int_distribution<int> ALPHANUM_DIST(0, sizeof(ALPHANUM) - 1);
            static std::uniform_int_distribution<int> ud(0, MAX_SIZE);

            size_t size = ud(engine);
            std::string str;
            str.reserve(size);
            for (size_t i = 0; i < size; ++i) {
                str += ALPHANUM[ALPHANUM_DIST(engine)];
            }

            return Builder {}.setString(str);
        }

        Builder& setString(std::string_view value) noexcept
        {
            b_str = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }

            stream.storeString((symbol_t*)b_str.data(), b_str.size());
        }

    private:
        std::string b_str;
    };

private:
    ArrayBase(VectorImpl<symbol_t>&& data) noexcept
        : m_data(std::move(data))
    {}

    VectorImpl<symbol_t> m_data;
};



template <bool BOXED, Primitive T>
class ArrayBase<BOXED, Scalar<T>> {
public:
    static constexpr Magic MAGIC = utils::commutative(1351535537, Scalar<T>::MAGIC);
    static constexpr bool STATIC = false;

    ArrayBase() noexcept = default;

    ArrayBase(const ArrayBase&) noexcept = default;
    ArrayBase(ArrayBase&&) noexcept = default;
    ArrayBase& operator=(const ArrayBase&) noexcept = default;
    ArrayBase& operator=(ArrayBase&&) noexcept = default;

    ~ArrayBase() noexcept = default;



    Nat size() const noexcept
    {
        return m_data.size();
    }



    const T& operator[](Nat idx) const noexcept
    {
        return m_data[idx];
    }



    auto begin() const noexcept
    {
        return m_data.begin();
    }



    auto end() const noexcept
    {
        return m_data.end();
    }



    static ArrayBase fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }

        VectorImpl<T> data = stream.fetchArray<T>();
        return {std::move(data)};
    }



    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }

        stream.storeArray(m_data.data(), m_data.size());
    }



    class Builder {
    public:
        using TYPE = ArrayBase;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const ArrayBase<RHS_BOXED, Scalar<T>>& rhs) noexcept
        {
            if (lhs.b_arr.size() != rhs.size())
                return false;

            for (size_t i = 0; i < rhs.size(); ++i) {
                if (lhs.b_arr[i] != rhs[i])
                    return false;
            }

            return true;
        }

        template <size_t MAX_SIZE>
        requires(std::is_integral_v<T>)
        static Builder random(std::default_random_engine& engine) noexcept
        {
            static std::uniform_int_distribution<int> ud(0, MAX_SIZE);
            size_t size = ud(engine);
            std::vector<T> vec;
            vec.reserve(size);

            static std::uniform_int_distribution<T> tud(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
            for (size_t i = 0; i < size; ++i) {
                vec.push_back(tud(engine));
            }

            return Builder {}.setArray(vec);
        }

        template <size_t MAX_SIZE>
        requires(std::is_floating_point_v<T>)
        static Builder random(std::default_random_engine& engine) noexcept
        {
            static std::uniform_int_distribution<int> ud(0, MAX_SIZE);
            size_t size = ud(engine);
            std::vector<T> vec;
            vec.reserve(size);

            static std::uniform_real_distribution<T> tud(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
            for (size_t i = 0; i < size; ++i) {
                vec.push_back(tud(engine));
            }

            return Builder {}.setArray(std::move(vec));
        }

        Builder& setArray(const std::vector<T>& value) noexcept
        {
            b_arr = value;
            return *this;
        }

        Builder& setArray(std::vector<T>&& value) noexcept
        {
            b_arr = std::move(value);
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }

            stream.storeArray(b_arr.data(), b_arr.size());
        }

    private:
        std::vector<T> b_arr;
    };

private:
    ArrayBase(VectorImpl<T>&& data) noexcept
        : m_data(std::move(data))
    {}

    VectorImpl<T> m_data;
};



template <bool BOXED, TLType T>
requires(!Primitive<T> && !std::is_same_v<T, symbol_t>)
class ArrayBase<BOXED, T> {
public:
    static constexpr Magic MAGIC = utils::commutative(1351535537, T::MAGIC);
    static constexpr bool STATIC = false;

    ArrayBase() noexcept = default;

    ArrayBase(const ArrayBase&) noexcept = default;
    ArrayBase(ArrayBase&&) noexcept = default;
    ArrayBase& operator=(const ArrayBase&) noexcept = default;
    ArrayBase& operator=(ArrayBase&&) noexcept = default;

    ~ArrayBase() noexcept = default;



    Nat size() const noexcept
    {
        return m_data.size();
    }



    const T& operator[](Nat idx) const noexcept
    {
        return m_data[idx];
    }



    auto begin() const noexcept
    {
        return m_data.begin();
    }



    auto end() const noexcept
    {
        return m_data.end();
    }



    template <typename... ARGS>
    static ArrayBase fetch(InputStream& stream, ARGS&&... args)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }

        Nat size = Nat::fetch(stream);
        VectorImpl<T> data(size);
        for (size_t i = 0; i < size; ++i) {
            data[i] = T::fetch(stream, std::forward<ARGS>(args)...);
        }

        return {std::move(data)};
    }



    template <typename... ARGS>
    void store(OutputStream& stream, ARGS&&... args) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }

        ((Nat)m_data.size()).store(stream);
        for (size_t i = 0; i < m_data.size(); ++i) {
            m_data[i].store(stream, std::forward<ARGS>(args)...);
        }
    }



    class Builder {
    private:
        using TBuilder = typename T::Builder;

    public:
        using TYPE = ArrayBase;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const ArrayBase<RHS_BOXED, T>& rhs) noexcept
        {
            if (lhs.b_arr.size() != rhs.size())
                return false;

            for (size_t i = 0; i < rhs.size(); ++i) {
                if (lhs.b_arr[i] != rhs[i])
                    return false;
            }

            return true;
        }

        template <size_t MAX_SIZE>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            static std::uniform_int_distribution<int> ud(0, MAX_SIZE);
            size_t size = ud(engine);
            std::vector<TBuilder> vec;
            vec.reserve(size);

            for (size_t i = 0; i < size; ++i) {
                vec.push_back(TBuilder::random(engine));
            }

            return Builder {}.setArray(std::move(vec));
        }

        template <size_t MAX_SIZE, size_t... SIZES>
        static Builder random(std::default_random_engine& engine) noexcept
        requires(requires{TBuilder::template random<SIZES...>(engine);})
        {
            static std::uniform_int_distribution<int> ud(0, MAX_SIZE);
            size_t size = ud(engine);
            std::vector<TBuilder> vec;
            vec.reserve(size);

            for (size_t i = 0; i < size; ++i) {
                vec.push_back(TBuilder::template random<SIZES...>(engine));
            }

            return Builder {}.setArray(std::move(vec));
        }

        Builder& setArray(std::vector<TBuilder>& value) noexcept
        {
            b_arr = value;
            return *this;
        }

        Builder& setArray(std::vector<TBuilder>&& value) noexcept
        {
            b_arr = std::move(value);
            return *this;
        }

        template <typename... Args>
        void store(OutputStream& stream, Args&&... args) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }

            ((Nat)b_arr.size()).store(stream);
            for (size_t i = 0; i < b_arr.size(); ++i) {
                b_arr[i].store(stream, std::forward<Args>(args)...);
            }
        }

    private:
        std::vector<TBuilder> b_arr;
    };

private:
    ArrayBase(VectorImpl<T>&& data) noexcept
        : m_data(std::move(data))
    {}

    VectorImpl<T> m_data;
};



template <TLType T>
using Array = ArrayBase<true, T>;

template <TLType T>
using array = ArrayBase<false, T>;

using String = Array<symbol_t>;
using string = array<symbol_t>;



template <TLType T, bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const ArrayBase<LHS_BOXED, T>& lhs, const ArrayBase<RHS_BOXED, T>& rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i])
            return false;
    }

    return true;
}



template <bool BOXED>
bool operator==(const ArrayBase<BOXED, symbol_t>& lhs, std::string_view rhs) noexcept
{
    if (lhs.size() != rhs.size())
        return false;

    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i])
            return false;
    }
    return true;
}



template <typename T, typename U, typename A, bool BOXED>
bool operator==(const ArrayBase<BOXED, T>& lhs, const std::vector<U, A>& rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i])
            return false;
    }

    return true;
}



template <bool BOXED>
size_t consume(const ArrayBase<BOXED, symbol_t>& value) noexcept
{
    size_t result = 0;
    for (auto it : value) {
        result += it;
    }
    return result;
}



template <bool BOXED, Primitive T>
size_t consume(const ArrayBase<BOXED, Scalar<T>>& value) noexcept
{
    size_t result = 0;
    for (const auto& it : value) {
        result += it;
    }
    return result;
}



template <bool BOXED, TLType T>
requires(!Primitive<T> && !std::is_same_v<T, symbol_t>)
size_t consume(const ArrayBase<BOXED, T>& value) noexcept
{
    size_t result = 0;
    for (const auto& it : value) {
        result += consume(it);
    }
    return result;
}

}    // namespace baseline
