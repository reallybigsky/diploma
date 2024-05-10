#pragma once

#include "prelude/Common.hpp"
#include "prelude/opus/Allocator.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>



#define SHARED_MASK 0x8000000000000000
#define PTR_MASK    0x7FFFFFFFFFFFFFFF

#define MARK_SHARED(ptr) ((uintptr_t)(ptr) | SHARED_MASK)
#define IS_SHARED(ptr)   (!!((ptr) & SHARED_MASK))
#define GET_PTR(arg)     ((arg) & PTR_MASK)



namespace opus {

class InputStream;
class OutputStream;



class Proxy {
public:
    Proxy() noexcept = default;

    static Proxy create(size_t size) noexcept
    {
        uint8_t* buf = static_cast<uint8_t*>(Allocator::allocate(sizeof(ref_cnt_t) + size));
        ref_cnt_t* ref_cnt = (ref_cnt_t*)buf;
        *ref_cnt = 1;
        return Proxy {ref_cnt, buf + sizeof(ref_cnt_t)};
    }



    Proxy(void* ref_cnt, uint8_t* data) noexcept
        : m_ref_cnt((ref_cnt_t*)ref_cnt)
        , m_data(data)
    {}



    Proxy(const Proxy& other, offset_t offset) noexcept
        : m_ref_cnt(other.m_ref_cnt)
    {
        if (other.m_ref_cnt)
            *other.m_ref_cnt += 1;

        if (other.m_data)
            m_data = other.m_data + offset;
    }


    Proxy(const Proxy& other) noexcept
    {
        if (other.m_ref_cnt)
            *other.m_ref_cnt += 1;

        m_ref_cnt = other.m_ref_cnt;
        m_data = other.m_data;
    }



    Proxy(Proxy&& other) noexcept
    {
        std::swap(m_ref_cnt, other.m_ref_cnt);
        std::swap(m_data, other.m_data);
    }



    Proxy& operator=(const Proxy& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();
        if (other.m_ref_cnt)
            *other.m_ref_cnt += 1;

        m_ref_cnt = other.m_ref_cnt;
        m_data = other.m_data;
        return *this;
    }



    Proxy& operator=(Proxy&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();
        std::swap(m_ref_cnt, other.m_ref_cnt);
        std::swap(m_data, other.m_data);
        return *this;
    }



    ~Proxy() noexcept
    {
        clear();
    }


    static Proxy fetch(InputStream& stream, size_t size);
    void store(OutputStream& stream, size_t size) const;


    ref_cnt_t* refCntPtr() const noexcept
    {
        return m_ref_cnt;
    }



    uint8_t* data() const noexcept
    {
        return m_data;
    }



    explicit operator bool() const noexcept
    {
        return m_data != nullptr;
    }

private:
    void clear() noexcept
    {
        if (m_ref_cnt && --(*m_ref_cnt) == 0) {
            Allocator::deallocate(m_ref_cnt);
        }

        m_ref_cnt = nullptr;
        m_data = nullptr;
    }

private:
    ref_cnt_t* m_ref_cnt = nullptr;
    uint8_t* m_data = nullptr;
};



template <TLType T>
class ArrayProxy {
private:
    template <typename U>
    struct Table {
    public:
        ref_cnt_t p_ref_cnt;
        size_t p_size;
        uintptr_t p_data[];

    public:
        Table() = delete;

        Table(const Table&) = delete;
        Table(Table&&) = delete;
        Table& operator=(const Table&) = delete;
        Table& operator=(Table&&) = delete;

        static Table<U>* create(size_t size) noexcept
        requires(StaticType<U>)
        {
            Table<U>* proxy = static_cast<Table<U>*>(Allocator::allocate(sizeof(Table<U>) + 2 * sizeof(uintptr_t) * size));
            proxy->p_ref_cnt = 1;
            proxy->p_size = size;
            return proxy;
        }

        static Table<U>* create(size_t size) noexcept
        requires(DynamicType<U>)
        {
            Table<U>* proxy = static_cast<Table<U>*>(Allocator::allocate(sizeof(Table<U>) + sizeof(U) * size));
            proxy->p_ref_cnt = 1;
            proxy->p_size = size;
            return proxy;
        }

        ~Table() noexcept
        requires(std::is_same_v<symbol_t, U> || Primitive<U>)
        = default;

        ~Table() noexcept
        requires(Struct<U>)
        {
            for (size_t i = 0; i < 2 * p_size; i += 2) {
                if (p_data[i] == 1 && IS_SHARED(p_data[i + 1])) {
                    U* u = (U*)GET_PTR(p_data[i + 1]);
                    std::destroy_at(u);
                    Allocator::deallocate(u);
                }
            }
        }

        ~Table() noexcept
        requires(DynamicType<U>)
        {
            for (size_t i = 0; i < p_size; ++i) {
                std::destroy_at((U*)((uint8_t*)p_data + sizeof(U) * i));
            }
        }
    };

public:
    ArrayProxy() = default;

    explicit ArrayProxy(size_t table_size) noexcept
        : m_data(Table<T>::create(table_size))
    {}



    ArrayProxy(const ArrayProxy& other) noexcept
        : m_data(other.m_data)
    {
        if (other.m_data)
            ++other.m_data->p_ref_cnt;
    }



    ArrayProxy(ArrayProxy&& other) noexcept
    {
        std::swap(m_data, other.m_data);
    }



    ArrayProxy& operator=(const ArrayProxy& other) noexcept
    {
        if (this == &other)
            return *this;

        clearTable();
        m_data = other.m_data;
        if (other.m_data)
            ++other.m_data->p_ref_cnt;
        return *this;
    }



    ArrayProxy& operator=(ArrayProxy&& other) noexcept
    {
        if (this == &other)
            return *this;

        clearTable();
        std::swap(m_data, other.m_data);
        return *this;
    }



    ~ArrayProxy() noexcept
    {
        clearTable();
    }



    uintptr_t& operator[](size_t idx) noexcept
    {
        return m_data->p_data[idx];
    }



    const uintptr_t& operator[](size_t idx) const noexcept
    {
        return m_data->p_data[idx];
    }



    bool empty() const noexcept
    {
        return m_data == nullptr;
    }



    size_t size() const noexcept
    {
        return m_data ? m_data->p_size : 0;
    }



    uint8_t* data() const noexcept
    {
        return m_data ? (uint8_t*)m_data->p_data : nullptr;
    }



    uintptr_t* table() const noexcept
    {
        return m_data ? m_data->p_data : nullptr;
    }

private:
    void clearTable() noexcept
    {
        if (m_data && --m_data->p_ref_cnt == 0) {
            std::destroy_at(m_data);
            Allocator::deallocate(m_data);
        }

        m_data = nullptr;
    }

private:
    Table<T>* m_data = nullptr;
};

}    // namespace opus
