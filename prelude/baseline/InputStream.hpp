#pragma once

#include "prelude/Common.hpp"
#include "prelude/DataStream.hpp"
#include "prelude/baseline/Allocator.hpp"

#include <cstring>



namespace baseline {

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



    template <Primitive T>
    VectorImpl<T> fetchArray()
    {
        const uint32_t size = fetch<uint32_t>();
        const size_t byte_size = sizeof(T) * size;
        VectorImpl<T> result(size);
        for (size_t curr_byte_pos = 0, bytes_to_copy; curr_byte_pos < byte_size; curr_byte_pos += bytes_to_copy) {
            advanceBlockIfFull();
            bytes_to_copy = std::min(m_block->size - m_pos, byte_size - curr_byte_pos);
            memcpy((uint8_t*)result.data() + curr_byte_pos, m_block->data() + m_pos, bytes_to_copy);
            m_pos += bytes_to_copy;
        }

        return result;
    }



    template <typename T>
    requires(std::is_same_v<T, symbol_t>)
    VectorImpl<symbol_t> fetchArray()
    {
        return fetchString();
    }



    VectorImpl<symbol_t> fetchString()
    {
        advanceBlockIfFull();

        size_t size = fetchStringSize();
        VectorImpl<symbol_t> result(size);

        for (size_t curr_byte_pos = 0, bytes_to_copy; curr_byte_pos < size; curr_byte_pos += bytes_to_copy) {
            advanceBlockIfFull();
            bytes_to_copy = std::min(m_block->size - m_pos, size - curr_byte_pos);
            memcpy((uint8_t*)result.data() + curr_byte_pos, m_block->data() + m_pos, bytes_to_copy);
            m_pos += bytes_to_copy;
        }

        m_pos += (sizeof(uint32_t) - (m_pos % sizeof(uint32_t))) % sizeof(uint32_t);
        return result;
    }

private:
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

}    // namespace baseline
