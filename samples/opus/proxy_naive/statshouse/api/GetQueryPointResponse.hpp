#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/proxy_naive/statshouse/api/PointMeta.hpp"



namespace opus::proxy_naive::statshouse {

class GetQueryPointResponse {
public:
    static constexpr Magic MAGIC = 2038907665;

    GetQueryPointResponse() noexcept = default;

    GetQueryPointResponse(const GetQueryPointResponse&) noexcept = default;
    GetQueryPointResponse(GetQueryPointResponse&&) noexcept = default;
    GetQueryPointResponse& operator=(const GetQueryPointResponse&) noexcept = default;
    GetQueryPointResponse& operator=(GetQueryPointResponse&&) noexcept = default;

    ~GetQueryPointResponse() noexcept = default;

    bool verify() const noexcept
    {
        if (!get_fields_mask().verify())
            return false;

        return true;
    }

    Nat get_fields_mask() const noexcept
    {
        return Nat {m_proxy_1, PROXY_1_FIELDS_MASK_OFFSET};
    }

    const array<Double>& get_data() const noexcept
    {
        return m_data;
    }

    const array<pointMeta>& get_meta() const noexcept
    {
        return m_meta;
    }

    static GetQueryPointResponse fetch(InputStream& stream)
    {
        Proxy proxy_1 = Proxy::fetch(stream, PROXY_1_SIZEOF);
        array<Double> data = array<Double>::fetch(stream);
        array<pointMeta> meta = array<pointMeta>::fetch(stream);
        GetQueryPointResponse result(std::move(proxy_1),
                                     std::move(data),
                                     std::move(meta));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_proxy_1.store(stream, PROXY_1_SIZEOF);
        m_data.store(stream);
        m_meta.store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const GetQueryPointResponse& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_data == rhs.get_data()
                   && lhs.b_meta == rhs.get_meta();
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3, size_t SIZE_4, size_t SIZE_5>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(Nat::Builder::random(engine))
                    .set_data(array<Double>::Builder::random<SIZE_1>(engine))
                    .set_meta(array<pointMeta>::Builder::random<SIZE_2, SIZE_3, SIZE_4, SIZE_5>(engine));
        }

        Builder& set_fields_mask(const Nat::Builder& value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_data(const array<Double>::Builder& value) noexcept
        {
            b_data = value;
            return *this;
        }

        Builder& set_meta(const array<pointMeta>::Builder& value) noexcept
        {
            b_meta = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            MAGIC.store(stream);
            b_fields_mask.store(stream);
            b_data.store(stream);
            b_meta.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        array<Double>::Builder b_data;
        array<pointMeta>::Builder b_meta;
    };

private:
    static constexpr offset_t PROXY_1_SIZEOF = Nat::SIZEOF;
    static constexpr offset_t PROXY_1_FIELDS_MASK_OFFSET = 0;

    GetQueryPointResponse(Proxy&& proxy_1,
                          array<Double>&& data,
                          array<pointMeta>&& meta) noexcept
        : m_proxy_1(std::move(proxy_1))
        , m_data(std::move(data))
        , m_meta(std::move(meta))
    {}

    Proxy m_proxy_1;
    array<Double> m_data;
    array<pointMeta> m_meta;
};

bool operator==(const GetQueryPointResponse& lhs, const GetQueryPointResponse& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_data() == rhs.get_data()
           && lhs.get_meta() == rhs.get_meta();
}

size_t consume(const GetQueryPointResponse& value) noexcept
{
    size_t result = 0;
    result += consume(value.get_fields_mask());
    result += consume(value.get_data());
    result += consume(value.get_meta());
    return result;
}

}    // namespace opus::proxy_naive::statshouse
