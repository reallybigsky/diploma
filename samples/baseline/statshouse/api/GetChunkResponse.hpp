#pragma once

#include "prelude/baseline/Includes.hpp"

#include "samples/baseline/statshouse/api/Series.hpp"



namespace baseline::statshouse {

class GetChunkResponse {
public:
    static constexpr Magic MAGIC = 4269532675;

    GetChunkResponse() noexcept = default;

    GetChunkResponse(const GetChunkResponse&) noexcept = default;
    GetChunkResponse(GetChunkResponse&&) noexcept = default;
    GetChunkResponse& operator=(const GetChunkResponse&) noexcept = default;
    GetChunkResponse& operator=(GetChunkResponse&&) noexcept = default;

    ~GetChunkResponse() noexcept = default;

    const Nat& get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const series& get_series() const noexcept
    {
        return m_series;
    }

    const Int& get_index() const noexcept
    {
        return m_index;
    }

    static GetChunkResponse fetch(InputStream& stream)
    {
        Nat fields_mask = Nat::fetch(stream);
        series series = series::fetch(stream);
        Int index = Int::fetch(stream);
        GetChunkResponse result(std::move(fields_mask),
                                std::move(series),
                                std::move(index));
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_fields_mask.store(stream);
        m_series.store(stream);
        m_index.store(stream);
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
    GetChunkResponse(Nat&& fields_mask,
                     series&& series,
                     Int&& index) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_series(std::move(series))
        , m_index(std::move(index))
    {}

    Nat m_fields_mask;
    series m_series;
    Int m_index;
};

bool operator==(const GetChunkResponse& lhs, const GetChunkResponse& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_series() == rhs.get_series()
           && lhs.get_index() == rhs.get_index();
}

}    // namespace baseline::statshouse
