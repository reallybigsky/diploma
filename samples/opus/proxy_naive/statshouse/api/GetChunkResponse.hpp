#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/proxy_naive/statshouse/api/Series.hpp"



namespace opus::proxy_naive::statshouse {

class GetChunkResponse {
public:
    static constexpr Magic MAGIC = 4269532675;

    GetChunkResponse() noexcept = default;

    GetChunkResponse(const GetChunkResponse&) noexcept = default;
    GetChunkResponse(GetChunkResponse&&) noexcept = default;
    GetChunkResponse& operator=(const GetChunkResponse&) noexcept = default;
    GetChunkResponse& operator=(GetChunkResponse&&) noexcept = default;

    ~GetChunkResponse() noexcept = default;

    bool verify() const noexcept
    {
        if (!get_fields_mask().verify())
            return false;

        if (!get_index().verify())
            return false;

        return true;
    }

    Nat get_fields_mask() const noexcept
    {
        return Nat {m_proxy_1, PROXY_1_FIELDS_MASK_OFFSET};
    }

    const series& get_series() const noexcept
    {
        return m_series;
    }

    Int get_index() const noexcept
    {
        return Int {m_proxy_2, PROXY_2_INDEX_OFFSET};
    }

    static GetChunkResponse fetch(InputStream& stream)
    {
        Proxy proxy_1 = Proxy::fetch(stream, PROXY_1_SIZEOF);
        series series = series::fetch(stream);
        Proxy proxy_2 = Proxy::fetch(stream, PROXY_2_SIZEOF);
        GetChunkResponse result(std::move(proxy_1),
                                std::move(series),
                                std::move(proxy_2));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_proxy_1.store(stream, PROXY_1_SIZEOF);
        m_series.store(stream);
        m_proxy_2.store(stream, PROXY_2_SIZEOF);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const GetChunkResponse& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_series == rhs.get_series()
                   && lhs.b_index == rhs.get_index();
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(Nat::Builder::random(engine))
                    .set_series(series::Builder::random<SIZE_1, SIZE_2, SIZE_3>(engine))
                    .set_index(Int::Builder::random(engine));
        }

        Builder& set_fields_mask(const Nat::Builder& value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_series(const series::Builder& value) noexcept
        {
            b_series = value;
            return *this;
        }

        Builder& set_index(const Int::Builder& value) noexcept
        {
            b_index = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            MAGIC.store(stream);
            b_fields_mask.store(stream);
            b_series.store(stream);
            b_index.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        series::Builder b_series;
        Int::Builder b_index;
    };

private:
    static constexpr offset_t PROXY_1_SIZEOF = Nat::SIZEOF;
    static constexpr offset_t PROXY_1_FIELDS_MASK_OFFSET = 0;

    static constexpr offset_t PROXY_2_SIZEOF = Int::SIZEOF;
    static constexpr offset_t PROXY_2_INDEX_OFFSET = 0;

    GetChunkResponse(Proxy&& proxy_1,
                     series&& series,
                     Proxy&& proxy_2) noexcept
        : m_proxy_1(std::move(proxy_1))
        , m_series(std::move(series))
        , m_proxy_2(std::move(proxy_2))
    {}

    Proxy m_proxy_1;
    series m_series;
    Proxy m_proxy_2;
};

bool operator==(const GetChunkResponse& lhs, const GetChunkResponse& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_series() == rhs.get_series()
           && lhs.get_index() == rhs.get_index();
}

}    // namespace opus::proxy_naive::statshouse
