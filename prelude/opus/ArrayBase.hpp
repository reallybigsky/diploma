#pragma once

#include "prelude/Common.hpp"
#include "prelude/opus/InputStream.hpp"
#include "prelude/opus/OutputStream.hpp"
#include "prelude/opus/Proxy.hpp"

#include <optional>
#include <random>
#include <string>
#include <vector>



namespace opus {

template <bool BOXED, TLType T>
class ArrayBase;



template <bool BOXED>
class ArrayBase<BOXED, symbol_t> {
public:
    static constexpr Magic MAGIC = 106168542;
    static constexpr bool STATIC = false;

    ArrayBase() noexcept = default;

    ArrayBase(const ArrayBase& other) noexcept = default;
    ArrayBase(ArrayBase&& other) noexcept = default;
    ArrayBase& operator=(const ArrayBase& other) noexcept = default;
    ArrayBase& operator=(ArrayBase&& other) noexcept = default;

    ~ArrayBase() noexcept = default;



    size_t size() const noexcept
    {
        return m_size;
    }



    symbol_t operator[](size_t idx) const noexcept
    {
        uintptr_t* table = m_proxy.table();
        uintptr_t* currEntry = m_proxy.table();
        for (size_t i = 0; idx >= table[i]; i += 2) {
            idx -= table[i];
            currEntry += 2;
        }

        return ((symbol_t*)currEntry[1])[idx];
    }



    auto begin() const noexcept
    {
        return Iterator(*this);
    }



    auto end() const noexcept
    {
        return Iterator();
    }



    static ArrayBase fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }

        auto [proxy, size] = stream.fetchString();
        return {std::move(proxy), size};
    }



    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }

        stream.storeString(m_proxy, m_size);
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



    class Iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = symbol_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        Iterator(const ArrayBase<BOXED, symbol_t>& arr) noexcept
            : i_arr(arr)
            , i_outer_pos(0)
            , i_inner_pos(0)
            , i_entry(arr.m_proxy.table())
        {
            if (arr.size() == 0)
                *this = Iterator();
        }

        Iterator() noexcept = default;

        reference operator*() const noexcept
        {
            return ((value_type*)i_entry[1])[i_inner_pos];
        }

        pointer operator->() const noexcept
        {
            return ((value_type*)i_entry[1]) + i_inner_pos;
        }

        Iterator& operator++() noexcept
        {
            if (++i_outer_pos == i_arr.size()) {
                *this = Iterator();
                return *this;
            }

            if (++i_inner_pos == i_entry[0]) {
                i_inner_pos = 0;
                i_entry += 2;
            }

            return *this;
        }

        Iterator operator++(int) noexcept
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b) noexcept = default;

    private:
        ArrayBase<BOXED, symbol_t> i_arr;
        size_t i_outer_pos = 0;
        size_t i_inner_pos = 0;
        uintptr_t* i_entry = nullptr;
    };

private:
    ArrayBase(ArrayProxy<symbol_t>&& proxy, size_t size) noexcept
        : m_proxy(std::move(proxy))
        , m_size(size)
    {}

    ArrayProxy<symbol_t> m_proxy;
    size_t m_size = 0;
};



template <bool BOXED, Primitive T>
class ArrayBase<BOXED, Scalar<T>> {
public:
    static constexpr Magic MAGIC = utils::commutative(1351535537, Scalar<T>::MAGIC);
    static constexpr bool STATIC = false;

    ArrayBase() noexcept = default;

    ArrayBase(const ArrayBase& other) noexcept = default;
    ArrayBase(ArrayBase&& other) noexcept = default;
    ArrayBase& operator=(const ArrayBase& other) noexcept = default;
    ArrayBase& operator=(ArrayBase&& other) noexcept = default;

    ~ArrayBase() noexcept = default;



    size_t size() const noexcept
    {
        return m_size;
    }



    T operator[](size_t idx) const noexcept
    {
        uintptr_t* table = m_proxy.table();
        uintptr_t* currEntry = m_proxy.table();
        for (size_t i = 0; idx >= table[i]; i += 2) {
            idx -= table[i];
            currEntry += 2;
        }

        if (currEntry[0] == 1)
            return *(T*)(&currEntry[1]);

        return ((T*)currEntry[1])[idx];
    }



    auto begin() const noexcept
    {
        return Iterator(*this);
    }



    auto end() const noexcept
    {
        return Iterator();
    }



    static ArrayBase fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }

        Nat size = Nat::fetch(stream);
        ArrayProxy<T> proxy = stream.fetchArray<T>(size);

        return {std::move(proxy), size};
    }



    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }

        stream.storeArray(m_proxy, m_size);
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

            return Builder {}.setArray(vec);
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



    class Iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = const value_type*;
        using reference = const value_type&;

        Iterator(const ArrayBase<BOXED, Scalar<T>>& arr) noexcept
            : i_arr(arr)
            , i_outer_pos(0)
            , i_inner_pos(0)
            , i_entry(arr.m_proxy.table())
        {
            if (arr.size() == 0)
                *this = Iterator();
        }

        Iterator() noexcept = default;

        reference operator*() const noexcept
        {
            if (i_entry[0] == 1)
                return *(value_type*)(&i_entry[1]);

            return ((value_type*)i_entry[1])[i_inner_pos];
        }

        pointer operator->() const noexcept
        {
            if (i_entry[0] == 1)
                return (value_type*)(&i_entry[1]);

            return ((value_type*)i_entry[1]) + i_inner_pos;
        }

        Iterator& operator++() noexcept
        {
            if (++i_outer_pos == i_arr.size()) {
                *this = Iterator();
                return *this;
            }

            if (++i_inner_pos == i_entry[0]) {
                i_inner_pos = 0;
                i_entry += 2;
            }

            return *this;
        }

        Iterator operator++(int) noexcept
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b) noexcept = default;

    private:
        ArrayBase<BOXED, Scalar<T>> i_arr;
        size_t i_outer_pos = 0;
        size_t i_inner_pos = 0;
        uintptr_t* i_entry = nullptr;
    };

private:
    ArrayBase(ArrayProxy<T>&& proxy, Nat size) noexcept
        : m_proxy(std::move(proxy))
        , m_size(size)
    {}

    ArrayProxy<T> m_proxy;
    Nat m_size = 0;
};



template <bool BOXED, Struct T>
class ArrayBase<BOXED, T> {
public:
    static constexpr Magic MAGIC = utils::commutative(1351535537, T::MAGIC);
    static constexpr bool STATIC = false;

    ArrayBase() noexcept = default;

    ArrayBase(const ArrayBase& other) noexcept = default;
    ArrayBase(ArrayBase&& other) noexcept = default;
    ArrayBase& operator=(const ArrayBase& other) noexcept = default;
    ArrayBase& operator=(ArrayBase&& other) noexcept = default;

    ~ArrayBase() noexcept = default;



    bool verify() const noexcept
    {
        for (size_t i = 0; i < m_size; ++i) {
            if (!(*this)[i].verify())
                return false;
        }

        return true;
    }



    size_t size() const noexcept
    {
        return m_size;
    }



    T operator[](size_t idx) const noexcept
    {
        uintptr_t* table = m_proxy.table();
        uintptr_t* currEntry = m_proxy.table();
        for (size_t i = 0; idx >= table[i]; i += 2) {
            idx -= table[i];
            currEntry += 2;
        }

        if (currEntry[0] == 1 && IS_SHARED(currEntry[1])) {
            T t = *(T*)GET_PTR(currEntry[1]);
            return t;
        }

        Proxy proxy(nullptr, (uint8_t*)currEntry[1] + T::SIZEOF * idx);
        return T {proxy, 0};
    }



    auto begin() const noexcept
    {
        return Iterator(*this);
    }



    auto end() const noexcept
    {
        return Iterator();
    }



    static ArrayBase fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }

        Nat size = Nat::fetch(stream);
        ArrayProxy<T> proxy = stream.fetchArray<T>(size);

        ArrayBase result(std::move(proxy), size);
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);

        return result;
    }



    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }

        stream.storeArray(m_proxy, m_size);
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

        Builder& setArray(const std::vector<TBuilder>& value) noexcept
        {
            b_arr = value;
            return *this;
        }

        Builder& setArray(std::vector<TBuilder>&& value) noexcept
        {
            b_arr = std::move(value);
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }

            ((Nat)b_arr.size()).store(stream);
            for (size_t i = 0; i < b_arr.size(); ++i) {
                b_arr[i].store(stream);
            }
        }

    private:
        std::vector<TBuilder> b_arr;
    };



    class Iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = Iterator&;
        using reference = value_type;

        Iterator(const ArrayBase<BOXED, T>& arr) noexcept
            : i_arr(arr)
            , i_outer_pos(0)
            , i_inner_pos(0)
            , i_entry(arr.m_proxy.table())
        {
            if (arr.size() == 0)
                *this = Iterator();
        }

        Iterator() noexcept = default;

        reference operator*() const noexcept
        {
            if (i_entry[0] == 1 && IS_SHARED(i_entry[1]))
                return *(T*)GET_PTR(i_entry[1]);

            Proxy proxy(nullptr, (uint8_t*)i_entry[1] + T::SIZEOF * i_inner_pos);
            return T {proxy, 0};
        }

        pointer operator->() const noexcept
        {
            return *this;
        }

        operator reference() const noexcept
        {
            return this->operator*();
        }

        Iterator& operator++() noexcept
        {
            if (++i_outer_pos == i_arr.size()) {
                *this = Iterator();
                return *this;
            }

            if (++i_inner_pos == i_entry[0]) {
                i_inner_pos = 0;
                i_entry += 2;
            }

            return *this;
        }

        Iterator operator++(int) noexcept
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b) noexcept = default;

    private:
        ArrayBase<BOXED, T> i_arr;
        size_t i_outer_pos = 0;
        size_t i_inner_pos = 0;
        uintptr_t* i_entry = nullptr;
    };

private:
    ArrayBase(ArrayProxy<T>&& proxy, Nat size) noexcept
        : m_proxy(std::move(proxy))
        , m_size(size)
    {}

    ArrayProxy<T> m_proxy;
    Nat m_size = 0;
};



template <bool BOXED, DynamicType T>
class ArrayBase<BOXED, T> {
public:
    static constexpr Magic MAGIC = 0;
    static constexpr bool STATIC = false;

    ArrayBase() noexcept = default;

    ArrayBase(const ArrayBase& other) noexcept = default;
    ArrayBase(ArrayBase&& other) noexcept = default;
    ArrayBase& operator=(const ArrayBase& other) noexcept = default;
    ArrayBase& operator=(ArrayBase&& other) noexcept = default;

    ~ArrayBase() = default;



    bool verify() const noexcept
    {
        return true;
    }



    size_t size() const noexcept
    {
        return m_proxy.size();
    }



    const T& operator[](size_t idx) const noexcept
    {
        return *(T*)(m_proxy.data() + sizeof(T) * idx);
    }



    auto begin() const noexcept
    {
        return Iterator(*this);
    }



    auto end() const noexcept
    {
        return Iterator();
    }



    template <typename... ARGS>
    static ArrayBase fetch(InputStream& stream, ARGS&&... args)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }

        Nat size = Nat::fetch(stream);
        ArrayProxy<T> proxy(size);
        for (size_t i = 0; i < size; ++i) {
            T* arena = (T*)(proxy.data() + sizeof(T) * i);
            std::construct_at(arena, T::fetch(stream, std::forward<ARGS>(args)...));
        }

        return {std::move(proxy)};
    }



    template <typename... ARGS>
    void store(OutputStream& stream, ARGS&&... args) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }

        ((Nat)m_proxy.size()).store(stream);
        for (size_t i = 0; i < size(); ++i) {
            (*this)[i].store(stream, std::forward<ARGS>(args)...);
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
        requires(requires { TBuilder::template random<SIZES...>(engine); })
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

        Builder& setArray(const std::vector<TBuilder>& value) noexcept
        {
            b_arr = value;
            return *this;
        }

        Builder& setArray(std::vector<TBuilder>&& value) noexcept
        {
            b_arr = std::move(value);
            return *this;
        }

        template <typename... ARGS>
        void store(OutputStream& stream, ARGS&&... args) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }

            ((Nat)b_arr.size()).store(stream);
            for (size_t i = 0; i < b_arr.size(); ++i) {
                b_arr[i].store(stream, std::forward<ARGS>(args)...);
            }
        }

    private:
        std::vector<TBuilder> b_arr;
    };



    class Iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = const value_type*;
        using reference = const value_type&;

        Iterator(const ArrayBase<BOXED, T>& arr) noexcept
            : i_arr(arr)
            , i_pos(0)
        {
            if (arr.size() == 0)
                *this = Iterator();
        }

        Iterator() noexcept = default;

        reference operator*() const noexcept
        {
            return i_arr[i_pos];
        }

        pointer operator->() const noexcept
        {
            return &i_arr[i_pos];
        }

        Iterator& operator++() noexcept
        {
            if (++i_pos == i_arr.size()) {
                *this = Iterator();
                return *this;
            }

            return *this;
        }

        Iterator operator++(int) noexcept
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b) noexcept = default;

    private:
        ArrayBase<BOXED, T> i_arr;
        size_t i_pos = 0;
    };

private:
    ArrayBase(ArrayProxy<T>&& proxy) noexcept
        : m_proxy(std::move(proxy))
    {}

    ArrayProxy<T> m_proxy;
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



template <typename T, typename U, bool BOXED>
bool operator==(const ArrayBase<BOXED, T>& lhs, const std::vector<U>& rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i])
            return false;
    }

    return true;
}

}    // namespace opus
