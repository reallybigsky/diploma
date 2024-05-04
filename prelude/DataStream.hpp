#pragma once

#include "prelude/Common.hpp"

#include <exception>
#include <list>



class DataStream {
public:
    struct Block {
        std::shared_ptr<uint8_t[]> sptr;
        size_t size;

        uint8_t* data() const
        {
            return sptr.get();
        }
    };

    using Iterator = std::list<Block>::const_iterator;

    DataStream() noexcept
        : m_queue()
        , m_remaining_bytes(0)
        , m_block()
        , m_pos(0)
    {}

    DataStream(const DataStream&) = delete;
    DataStream& operator=(const DataStream&) = delete;

    DataStream(DataStream&&) noexcept = default;
    DataStream& operator=(DataStream&&) noexcept = default;

    DataStream(std::initializer_list<Block> init) noexcept
        : m_queue(init)
        , m_remaining_bytes(0)
        , m_block(m_queue.cbegin())
        , m_pos(0)
    {
        for (auto b : init) {
            m_remaining_bytes += b.size;
        }

        static_assert(sizeof(int) == 4);
        static_assert(sizeof(float) == 4);
        static_assert(sizeof(long) == 8);
        static_assert(sizeof(double) == 8);

        static_assert(sizeof(size_t) == 8);

        static_assert(sizeof(void*) == 8);
        static_assert(sizeof(uintptr_t) == 8);
    }



    template <typename It>
    DataStream(It begin, It end) noexcept
        : m_queue(begin, end)
        , m_remaining_bytes(0)
        , m_block(m_queue.cbegin())
        , m_pos(0)
    {
        for (auto it = begin; it != end; ++it) {
            m_remaining_bytes += it->size;
        }
    }



    size_t pos() const noexcept
    {
        return m_pos;
    }



    size_t remainingBytes() const noexcept
    {
        return m_remaining_bytes - m_pos;
    }



    void add(const Block& block) noexcept
    {
        m_remaining_bytes += block.size;

        if (!m_queue.empty())
            return m_queue.push_back(block);

        m_queue.push_back(block);
        m_block = m_queue.cbegin();
    }



    template <typename It>
    void add(It begin, It end) noexcept
    {
        for (auto it = begin; it != end; ++it) {
            m_remaining_bytes += it->size;
        }

        if (!m_queue.empty())
            return m_queue.insert(m_queue.end(), begin, end);

        m_queue.insert(m_queue.end(), begin, end);
        m_block = m_queue.cbegin();
    }

protected:
    void advanceBlock(size_t new_pos = 0)
    {
        size_t prev_block_size = m_block->size;
        if (m_block == m_queue.cend() || ++m_block == m_queue.cend())
            throw TLException(TLException::TYPE::END);

        m_queue.pop_front();
        m_pos = new_pos;
        m_remaining_bytes = m_remaining_bytes - prev_block_size;
    }



    void advanceBlockIfFull()
    {
        size_t prev_block_size = m_block->size;
        if (m_block == m_queue.cend())
            throw TLException(TLException::TYPE::END);

        if (m_pos == m_block->size) {
            if (++m_block == m_queue.cend())
                throw TLException(TLException::TYPE::END);

            m_queue.pop_front();
            m_pos = 0;
            m_remaining_bytes = m_remaining_bytes - prev_block_size;
        }
    }



    void advanceBlockCopy(Iterator& block_copy, size_t& pos, size_t new_pos) const
    {
        if (block_copy == m_queue.cend() || ++block_copy == m_queue.cend())
            throw TLException(TLException::TYPE::END);

        pos = new_pos;
    }



    void advanceBlockCopyIfFull(Iterator& block_copy, size_t& pos) const
    {
        if (block_copy == m_queue.cend())
            throw TLException(TLException::TYPE::END);

        if (pos == block_copy->size) {
            if (++block_copy == m_queue.cend())
                throw TLException(TLException::TYPE::END);

            pos = 0;
        }
    }

private:
    std::list<Block> m_queue;
    size_t m_remaining_bytes;

protected:
    static constexpr uint8_t SMALL_STRING_BYTE = 0xFE;
    static constexpr uint8_t BIG_STRING_BYTE = 0xFF;

    static constexpr uint32_t MAX_SMALL_STRING_SIZE = 0xFE;
    static constexpr uint32_t MAX_BIG_STRING_SIZE = 1 << 24;

    Iterator m_block;
    size_t m_pos;
};