#pragma once

#include "baseline/Includes.hpp"
#include "baseline/statshouse/kv_engine/MetaInfo.hpp"



namespace baseline::statshouse {

class GetResponse {
public:
    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
    static constexpr bool CTS = true;

    GetResponse() noexcept = default;

    GetResponse(const GetResponse&) noexcept = default;
    GetResponse(GetResponse&&) noexcept = default;
    GetResponse& operator=(const GetResponse&) noexcept = default;
    GetResponse& operator=(GetResponse&&) noexcept = default;

    ~GetResponse() noexcept = default;

    GetResponse(const MetaInfo& meta,
                Long value) noexcept
        : m_meta(meta)
        , m_value(value)
    {}

    const MetaInfo& get_meta() const noexcept
    {
        return m_meta;
    }

    Long get_value() const noexcept
    {
        return m_value;
    }

    static GetResponse fetch(InputStream& stream)
    {
        MetaInfo meta = MetaInfo::fetch(stream);
        Long value = Long::fetch(stream);
        return {meta,
                value};
    }

    void store(OutputStream& stream) const
    {
        m_meta.store(stream);
        m_value.store(stream);
    }

    class Builder {
    public:
        Builder& set_meta(const MetaInfo::Builder& value) noexcept
        {
            b_meta = value;
            return *this;
        }

        Builder& set_value(Long::Builder value) noexcept
        {
            b_value = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            b_meta.store(stream);
            b_value.store(stream);
        }

    private:
        MetaInfo::Builder b_meta;
        Long::Builder b_value;
    };

private:
    MetaInfo m_meta;
    Long m_value;
};

}    // namespace baseline::statshouse