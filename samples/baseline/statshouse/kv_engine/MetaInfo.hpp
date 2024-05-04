#pragma once

#include "baseline/Includes.hpp"



namespace baseline::statshouse {

class MetaInfo {
public:
    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
    static constexpr bool CTS = true;

    MetaInfo() noexcept = default;

    MetaInfo(const MetaInfo&) noexcept = default;
    MetaInfo(MetaInfo&&) noexcept = default;
    MetaInfo& operator=(const MetaInfo&) noexcept = default;
    MetaInfo& operator=(MetaInfo&&) noexcept = default;

    ~MetaInfo() noexcept = default;

    MetaInfo(Long db_offset,
             Long committed_offset) noexcept
        : m_db_offset(db_offset)
        , m_committed_offset(committed_offset)
    {}

    Long get_db_offset() const noexcept
    {
        return m_db_offset;
    }

    Long get_committed_offset() const noexcept
    {
        return m_committed_offset;
    }

    static MetaInfo fetch(InputStream& stream)
    {
        Long db_offset = Long::fetch(stream);
        Long committed_offset = Long::fetch(stream);
        return {db_offset,
                committed_offset};
    }

    void store(OutputStream& stream) const
    {
        m_db_offset.store(stream);
        m_committed_offset.store(stream);
    }

    class Builder {
    public:
        Builder& set_db_offset(Long::Builder value) noexcept
        {
            b_db_offset = value;
            return *this;
        }

        Builder& set_committed_offset(Long::Builder value) noexcept
        {
            b_committed_offset = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            b_db_offset.store(stream);
            b_committed_offset.store(stream);
        }

    private:
        Long::Builder b_db_offset;
        Long::Builder b_committed_offset;
    };

private:
    Long m_db_offset;
    Long m_committed_offset;
};

}    // namespace baseline::statshouse
