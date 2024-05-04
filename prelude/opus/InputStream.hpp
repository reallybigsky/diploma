#pragma once

#include "prelude/Common.hpp"
#include "prelude/DataStream.hpp"
#include "prelude/opus/Proxy.hpp"

#include <cstring>



namespace opus {

class InputStream : public DataStream {
public:
    InputStream() = default;

    explicit InputStream(DataStream&& ds) noexcept
        : DataStream(std::move(ds))
    {}

    InputStream(std::initializer_list<Block> init) noexcept
        : DataStream(init)
    {}

    template <typename It>
    InputStream(It begin, It end) noexcept
        : DataStream(begin, end)
    {}



    template <Primitive T>
    requires(sizeof(T) == sizeof(uint32_t))
    T fetch()
    {
        advanceBlockIfFull();
        T result = *(T*)(m_block->data() + m_pos);
        m_pos += sizeof(T);
        return result;
    }



    template <Primitive T>
    requires(sizeof(T) == sizeof(uint64_t))
    T fetch()
    {
        advanceBlockIfFull();
        if (m_block->size - m_pos >= sizeof(T)) {
            T result = *(T*)(m_block->data() + m_pos);
            m_pos += sizeof(T);
            return result;
        }

        union {
            uint32_t hs[2];
            T value;
        } result = {};

        result.hs[0] = *(uint32_t*)(m_block->data() + m_pos);
        advanceBlock(sizeof(uint32_t));
        result.hs[1] = *(uint32_t*)(m_block->data());
        return result.value;
    }



    Proxy fetchData(uint32_t size)
    {
        if (m_block->size - m_pos >= size) {
            uint8_t* ptr = m_block->data() + m_pos;
            m_pos += size;
            return Proxy {nullptr, ptr};
        }

        Proxy proxy = Proxy::create(size);
        for (size_t curr_byte_pos = 0, bytes_to_copy; curr_byte_pos < size; curr_byte_pos += bytes_to_copy) {
            advanceBlockIfFull();
            bytes_to_copy = std::min(m_block->size - m_pos, size - curr_byte_pos);
            memcpy(proxy.data() + curr_byte_pos, m_block->data() + m_pos, bytes_to_copy);
            m_pos += bytes_to_copy;
        }

        return proxy;
    }



    template <Primitive T>
    ArrayProxy<T> fetchArray(size_t size)
    {
        const size_t num_chunks = getChunksCnt<sizeof(T)>(size);

        ArrayProxy<T> proxy(num_chunks);
        size_t curr_size = 0;

        for (size_t i = 0, tail; i < num_chunks; ++i) {
            advanceBlockIfFull();
            tail = std::min((size - curr_size) * sizeof(T), m_block->size - m_pos);

            if (tail >= 2 * sizeof(T)) {
                proxy[2 * i] = tail / sizeof(T);
                proxy[2 * i + 1] = (uintptr_t)(m_block->data() + m_pos);
                m_pos += tail / sizeof(T) * sizeof(T);
                curr_size += tail / sizeof(T);
                continue;
            }

            *(T*)(&proxy[2 * i + 1]) = fetch<T>();
            proxy[2 * i] = 1;
            ++curr_size;
        }

        return proxy;
    }



    template <Struct T>
    ArrayProxy<T> fetchArray(size_t size)
    {
        if constexpr (T::SIZEOF == 0) {
            ArrayProxy<T> proxy(1);
            proxy[0] = size;
            proxy[1] = 0;
            return proxy;
        }

        const size_t num_chunks = getChunksCnt<T::SIZEOF>(size);

        ArrayProxy<T> proxy(num_chunks);
        size_t curr_size = 0;

        for (size_t i = 0, tail; i < num_chunks; ++i) {
            advanceBlockIfFull();
            tail = std::min((size - curr_size) * T::SIZEOF, m_block->size - m_pos);

            if (tail >= T::SIZEOF) {
                proxy[2 * i] = tail / T::SIZEOF;
                proxy[2 * i + 1] = (uintptr_t)(m_block->data() + m_pos);
                m_pos += tail / T::SIZEOF * T::SIZEOF;
                curr_size += tail / T::SIZEOF;
                continue;
            }

            T* arena = (T*)alloc::allocate(sizeof(T));
            T* t_proxy = std::construct_at(arena, T::fetch(*this));

            proxy[2 * i] = 1;
            proxy[2 * i + 1] = MARK_SHARED(t_proxy);
            ++curr_size;
        }

        return proxy;
    }



    template <typename T>
    requires(std::is_same_v<T, symbol_t>)
    std::pair<ArrayProxy<symbol_t>, size_t> fetchArray()
    {
        return fetchString();
    }



    std::pair<ArrayProxy<symbol_t>, size_t> fetchString()
    {
        advanceBlockIfFull();

        size_t size = fetchStringSize();
        size_t num_chunks = getChunksCntString(size);
        ArrayProxy<symbol_t> proxy(num_chunks);
        size_t curr_size = 0;

        for (size_t i = 0, tail; i < num_chunks; ++i) {
            advanceBlockIfFull();
            tail = std::min(size - curr_size, m_block->size - m_pos);
            proxy[2 * i] = tail;
            proxy[2 * i + 1] = (uintptr_t)(m_block->data() + m_pos);
            m_pos += tail;
            curr_size += tail;
        }

        m_pos += (sizeof(uint32_t) - (m_pos % sizeof(uint32_t))) % sizeof(uint32_t);
        return std::make_pair(proxy, size);
    }



private:
    template <size_t SIZEOF>
    size_t getChunksCnt(size_t size) const
    {
        if constexpr (SIZEOF == 0) {
            return 0;
        }

        size_t result = 0;
        Iterator block_copy = m_block;
        size_t pos_copy = m_pos;
        for (size_t curr_size = 0, tail; curr_size < size; curr_size += tail / SIZEOF) {
            ++result;
            advanceBlockCopyIfFull(block_copy, pos_copy);
            tail = std::min((size - curr_size) * SIZEOF, block_copy->size - pos_copy);

            if (tail >= SIZEOF) {
                pos_copy += (tail / SIZEOF) * SIZEOF;
            } else {
                // Block Overlap
                advanceBlockCopy(block_copy, pos_copy, 0);
                for (size_t single_size = tail, single_tail; single_size < SIZEOF; single_size += single_tail) {
                    advanceBlockCopyIfFull(block_copy, pos_copy);
                    single_tail = std::min(SIZEOF - single_size, block_copy->size - pos_copy);
                    pos_copy += single_tail;
                }
                ++curr_size;
            }
        }

        return result;
    }



    size_t getChunksCntString(size_t size) const
    {
        size_t result = 0;

        Iterator block_copy = m_block;
        size_t pos_copy = m_pos;
        for (size_t curr_size = 0, tail; curr_size < size; curr_size += tail) {
            advanceBlockCopyIfFull(block_copy, pos_copy);
            ++result;
            tail = std::min(size - curr_size, block_copy->size - pos_copy);
            pos_copy += tail;
        }

        return result;
    }



    size_t fetchStringSize()
    {
        size_t result;
        uint8_t firstByte = *(uint8_t*)(m_block->data() + m_pos);

        if (firstByte < MAX_SMALL_STRING_SIZE) {
            result = firstByte;
            ++m_pos;
        } else if (firstByte < BIG_STRING_BYTE) {
            result = fetch<uint32_t>() >> 8;
        } else {
            result = fetch<uint64_t>() >> 8;
        }

        return result;
    }
};

Proxy Proxy::fetch(opus::InputStream& stream, size_t size)
{
    return stream.fetchData(size);
}

}    // namespace opus
