#pragma once

#include "prelude/baseline/Includes.hpp"

#include "samples/baseline/statshouse/api/PointMeta.hpp"



namespace baseline::statshouse {

class GetQueryPointResponse {
public:
    static constexpr Magic MAGIC = 2038907665;

    GetQueryPointResponse() noexcept = default;

    GetQueryPointResponse(const GetQueryPointResponse&) noexcept = default;
    GetQueryPointResponse(GetQueryPointResponse&&) noexcept = default;
    GetQueryPointResponse& operator=(const GetQueryPointResponse&) noexcept = default;
    GetQueryPointResponse& operator=(GetQueryPointResponse&&) noexcept = default;

    ~GetQueryPointResponse() noexcept = default;

    const Nat& get_fields_mask() const noexcept
    {
        return m_fields_mask;
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
        Nat fields_mask = Nat::fetch(stream);
        array<Double> data = array<Double>::fetch(stream);
        array<pointMeta> meta = array<pointMeta>::fetch(stream);
        GetQueryPointResponse result(std::move(fields_mask),
                                     std::move(data),
                                     std::move(meta));
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_fields_mask.store(stream);
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

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3, size_t SIZE_4>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(Nat::Builder::random(engine))
                    .set_data(array<Double>::Builder::random<SIZE_1>(engine))
                    .set_meta(array<pointMeta>::Builder::random<SIZE_2, SIZE_3, SIZE_4>(engine));
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
    GetQueryPointResponse(Nat&& fields_mask,
                          array<Double>&& data,
                          array<pointMeta>&& meta) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_data(std::move(data))
        , m_meta(std::move(meta))
    {}

    Nat m_fields_mask;
    array<Double> m_data;
    array<pointMeta> m_meta;
};

bool operator==(const GetQueryPointResponse& lhs, const GetQueryPointResponse& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_data() == rhs.get_data()
           && lhs.get_meta() == rhs.get_meta();
}

}    // namespace baseline::statshouse
