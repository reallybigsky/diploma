#pragma once

#include "prelude/Common.hpp"
#include "prelude/DataStream.hpp"
#include "prelude/opus/Proxy.hpp"

#include <cstring>



namespace opus {

class OutputStream : public DataStream {
public:
    OutputStream() = default;

    explicit OutputStream(DataStream&& ds) noexcept
        : DataStream(std::move(ds))
    {}

    OutputStream(std::initializer_list<Block> init) noexcept
        : DataStream(init)
    {}

    template <typename It>
    OutputStream(It begin, It end) noexcept
        : DataStream(begin, end)
    {}



    template <Primitive T>
    requires(sizeof(T) == sizeof(uint32_t))
    void store(T value)
    {
        advanceBlockIfFull();
        *(T*)(m_block->data() + m_pos) = value;
        m_pos += sizeof(T);
    }



    template <Primitive T>
    requires(sizeof(T) == sizeof(uint64_t))
    void store(T value)
    {
        advanceBlockIfFull();
        if (m_block->size - m_pos >= sizeof(T)) {
            *(T*)(m_block->data() + m_pos) = value;
            m_pos += sizeof(T);
            return;
        }

        union {
            uint32_t hs[2];
            T v;
        } tmp = {.v = value};

        *(uint32_t*)(m_block->data() + m_pos) = tmp.hs[0];
        advanceBlock(sizeof(uint32_t));
        *(uint32_t*)(m_block->data()) = tmp.hs[1];
    }



    void store(const Proxy& proxy, size_t size)
    {
        if (!proxy)
            return;

        for (size_t curr_byte_pos = 0, bytes_to_copy; curr_byte_pos < size; curr_byte_pos += bytes_to_copy) {
            advanceBlockIfFull();
            bytes_to_copy = std::min(m_block->size - m_pos, size - curr_byte_pos);
            memcpy(m_block->data() + m_pos, proxy.data() + curr_byte_pos, bytes_to_copy);
            m_pos += bytes_to_copy;
        }
    }



    template <typename T>
    requires(Primitive<T> || Struct<T>)
    void storeArray(const T* arr, uint32_t size)
    {
        store(size);

        if constexpr (Struct<T>)
            if constexpr (T::SIZEOF == 0)
                return;

        const size_t byte_size = sizeof(T) * size;
        for (size_t curr_byte_pos = 0, bytes_to_copy; curr_byte_pos < byte_size; curr_byte_pos += bytes_to_copy) {
            advanceBlockIfFull();
            bytes_to_copy = std::min(m_block->size - m_pos, byte_size - curr_byte_pos);
            memcpy(m_block->data() + m_pos, (uint8_t*)arr + curr_byte_pos, bytes_to_copy);
            m_pos += bytes_to_copy;
        }
    }



    template <Primitive T>
    void storeArray(const ArrayProxy<T>& proxy, uint32_t size)
    {
        store(size);

        for (size_t i = 0; i < proxy.size(); ++i) {
            if (proxy[2 * i] == 1) {
                T value = *(T*)&proxy[2 * i + 1];
                store(value);
                continue;
            }

            for (size_t curr_byte_pos = 0, bytes_to_copy; curr_byte_pos < proxy[2 * i] * sizeof(T); curr_byte_pos += bytes_to_copy) {
                advanceBlockIfFull();
                bytes_to_copy = std::min(m_block->size - m_pos, proxy[2 * i] * sizeof(T) - curr_byte_pos);
                memcpy(m_block->data() + m_pos, (uint8_t*)proxy[2 * i + 1] + curr_byte_pos, bytes_to_copy);
                m_pos += bytes_to_copy;
            }
        }
    }



    template <Struct T>
    void storeArray(const ArrayProxy<T>& proxy, uint32_t size)
    {
        store(size);

        if constexpr (T::SIZEOF == 0)
            return;

        for (size_t i = 0; i < proxy.size(); ++i) {
            if (proxy[2 * i] == 1 && IS_SHARED(proxy[2 * i + 1])) {
                T* t = (T*)GET_PTR(proxy[2 * i + 1]);
                t->store(*this);
                continue;
            }

            for (size_t curr_byte_pos = 0, bytes_to_copy; curr_byte_pos < proxy[2 * i] * T::SIZEOF; curr_byte_pos += bytes_to_copy) {
                advanceBlockIfFull();
                bytes_to_copy = std::min(m_block->size - m_pos, proxy[2 * i] * T::SIZEOF - curr_byte_pos);
                memcpy(m_block->data() + m_pos, (uint8_t*)proxy[2 * i + 1] + curr_byte_pos, bytes_to_copy);
                m_pos += bytes_to_copy;
            }
        }
    }




    void storeArray(const symbol_t* arr, size_t size)
    {
        storeString(arr, size);
    }



    void storeString(const symbol_t* arr, size_t size)
    {
        storeStringSize(size);

        for (size_t curr_byte_pos = 0, bytes_to_copy; curr_byte_pos < size; curr_byte_pos += bytes_to_copy) {
            advanceBlockIfFull();
            bytes_to_copy = std::min(m_block->size - m_pos, size - curr_byte_pos);
            memcpy(m_block->data() + m_pos, (uint8_t*)arr + curr_byte_pos, bytes_to_copy);
            m_pos += bytes_to_copy;
        }

        uint32_t padding = (sizeof(uint32_t) - (m_pos % sizeof(uint32_t))) % sizeof(uint32_t);
        if (padding > 0) {
            memset(m_block->data() + m_pos, 0, padding);
            m_pos += padding;
        }
    }



    void storeArray(const ArrayProxy<symbol_t>& proxy, size_t size)
    {
        storeString(proxy, size);
    }



    void storeString(const ArrayProxy<symbol_t>& proxy, size_t size)
    {
        storeStringSize(size);

        for (size_t i = 0; i < proxy.size(); ++i) {
            for (size_t curr_byte_pos = 0, bytes_to_copy; curr_byte_pos < proxy[2 * i]; curr_byte_pos += bytes_to_copy) {
                advanceBlockIfFull();
                bytes_to_copy = std::min(m_block->size - m_pos, proxy[2 * i] - curr_byte_pos);
                memcpy(m_block->data() + m_pos, (uint8_t*)proxy[2 * i + 1] + curr_byte_pos, bytes_to_copy);
                m_pos += bytes_to_copy;
            }
        }

        uint32_t padding = (sizeof(uint32_t) - (m_pos % sizeof(uint32_t))) % sizeof(uint32_t);
        if (padding > 0) {
            memset(m_block->data() + m_pos, 0, padding);
            m_pos += padding;
        }
    }

private:
    void storeStringSize(size_t size)
    {
        advanceBlockIfFull();

        if (size < MAX_SMALL_STRING_SIZE) {
            *(uint8_t*)(m_block->data() + m_pos) = (uint8_t)size;
            ++m_pos;
        } else if (size < MAX_BIG_STRING_SIZE) {
            store((uint32_t)((size << 8) | SMALL_STRING_BYTE));
        } else {
            store((uint64_t)((size << 8) | BIG_STRING_BYTE));
        }
    }
};

void Proxy::store(opus::OutputStream& stream, size_t size) const
{
    stream.store(*this, size);
}

}    // namespace opus
