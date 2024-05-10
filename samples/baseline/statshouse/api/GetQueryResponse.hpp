#pragma once

#include "prelude/baseline/Includes.hpp"

#include "samples/baseline/statshouse/api/Series.hpp"
#include "samples/baseline/statshouse/api/SeriesMeta.hpp"



namespace baseline::statshouse {

class GetQueryResponse {
public:
    static constexpr Magic MAGIC = 1148016255;

    GetQueryResponse() noexcept = default;

    GetQueryResponse(const GetQueryResponse&) noexcept = default;
    GetQueryResponse(GetQueryResponse&&) noexcept = default;
    GetQueryResponse& operator=(const GetQueryResponse&) noexcept = default;
    GetQueryResponse& operator=(GetQueryResponse&&) noexcept = default;

    ~GetQueryResponse() noexcept = default;

    const Nat& get_query_fields_mask() const noexcept
    {
        return m_query_fields_mask;
    }

    const Nat& get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const series& get_series() const noexcept
    {
        return m_series;
    }

    const array<seriesMeta>& get_series_meta() const noexcept
    {
        return m_series_meta;
    }

    const array<Int>& get_chunk_ids() const noexcept
    {
        return m_chunk_ids;
    }

    const Int& get_total_time_points() const noexcept
    {
        return m_total_time_points;
    }

    const Long& get_response_id() const noexcept
    {
        return m_response_id;
    }

    const std::optional<true_t>& get_excess_point_left() const noexcept
    {
        return m_excess_point_left;
    }

    const std::optional<true_t>& get_excess_point_right() const noexcept
    {
        return m_excess_point_right;
    }

    static GetQueryResponse fetch(InputStream& stream)
    {
        Nat query_fields_mask = Nat::fetch(stream);
        Nat fields_mask = Nat::fetch(stream);
        series series = series::fetch(stream);
        array<seriesMeta> series_meta = array<seriesMeta>::fetch(stream, query_fields_mask);
        array<Int> chunk_ids = array<Int>::fetch(stream);
        Int total_time_points = Int::fetch(stream);
        Long response_id = Long::fetch(stream);
        std::optional<true_t> excess_point_left;
        if (IS_SET(fields_mask, 0)) excess_point_left = true_t::fetch(stream);
        std::optional<true_t> excess_point_right;
        if (IS_SET(fields_mask, 1)) excess_point_right = true_t::fetch(stream);
        GetQueryResponse result(std::move(query_fields_mask),
                                std::move(fields_mask),
                                std::move(series),
                                std::move(series_meta),
                                std::move(chunk_ids),
                                std::move(total_time_points),
                                std::move(response_id),
                                std::move(excess_point_left),
                                std::move(excess_point_right));
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_query_fields_mask.store(stream);
        m_fields_mask.store(stream);
        m_series.store(stream);
        m_series_meta.store(stream, m_query_fields_mask);
        m_chunk_ids.store(stream);
        m_total_time_points.store(stream);
        m_response_id.store(stream);
        if (IS_SET(m_fields_mask, 0)) m_excess_point_left->store(stream);
        if (IS_SET(m_fields_mask, 1)) m_excess_point_right->store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const GetQueryResponse& rhs) noexcept
        {
            return lhs.b_query_fields_mask == rhs.get_query_fields_mask()
                   && lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_series == rhs.get_series()
                   && lhs.b_series_meta == rhs.get_series_meta()
                   && lhs.b_chunk_ids == rhs.get_chunk_ids()
                   && lhs.b_total_time_points == rhs.get_total_time_points()
                   && lhs.b_response_id == rhs.get_response_id()
                   && (!rhs.get_excess_point_left() || lhs.b_excess_point_left == rhs.get_excess_point_left())
                   && (!rhs.get_excess_point_right() || lhs.b_excess_point_right == rhs.get_excess_point_right());
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3, size_t SIZE_4, size_t SIZE_5, size_t SIZE_6, size_t SIZE_7, size_t SIZE_8, size_t SIZE_9, size_t SIZE_10, size_t SIZE_11, size_t SIZE_12>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_query_fields_mask(utils::random_mask<16, 511>(engine))
                    .set_fields_mask(utils::random_mask<0, 3>(engine))
                    .set_series(series::Builder::random<SIZE_1, SIZE_2, SIZE_3>(engine))
                    .set_series_meta(array<seriesMeta>::Builder::random<SIZE_4, SIZE_5, SIZE_6, SIZE_7, SIZE_8, SIZE_9, SIZE_10, SIZE_11>(engine))
                    .set_chunk_ids(array<Int>::Builder::random<SIZE_12>(engine))
                    .set_total_time_points(Int::Builder::random(engine))
                    .set_response_id(Long::Builder::random(engine))
                    .set_excess_point_left(true_t::Builder::random(engine))
                    .set_excess_point_right(true_t::Builder::random(engine));
        }

        Builder& set_query_fields_mask(const Nat::Builder& value) noexcept
        {
            b_query_fields_mask = value;
            return *this;
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

        Builder& set_series_meta(const array<seriesMeta>::Builder& value) noexcept
        {
            b_series_meta = value;
            return *this;
        }

        Builder& set_chunk_ids(const array<Int>::Builder& value) noexcept
        {
            b_chunk_ids = value;
            return *this;
        }

        Builder& set_total_time_points(const Int::Builder& value) noexcept
        {
            b_total_time_points = value;
            return *this;
        }

        Builder& set_response_id(const Long::Builder& value) noexcept
        {
            b_response_id = value;
            return *this;
        }

        Builder& set_excess_point_left(const true_t::Builder& value) noexcept
        {
            b_excess_point_left = value;
            return *this;
        }

        Builder& set_excess_point_right(const true_t::Builder& value) noexcept
        {
            b_excess_point_right = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            MAGIC.store(stream);
            b_query_fields_mask.store(stream);
            b_fields_mask.store(stream);
            b_series.store(stream);
            b_series_meta.store(stream, b_query_fields_mask);
            b_chunk_ids.store(stream);
            b_total_time_points.store(stream);
            b_response_id.store(stream);
            if (IS_SET(b_fields_mask, 0)) b_excess_point_left.store(stream);
            if (IS_SET(b_fields_mask, 1)) b_excess_point_right.store(stream);
        }

    private:
        Nat::Builder b_query_fields_mask;
        Nat::Builder b_fields_mask;
        series::Builder b_series;
        array<seriesMeta>::Builder b_series_meta;
        array<Int>::Builder b_chunk_ids;
        Int::Builder b_total_time_points;
        Long::Builder b_response_id;
        true_t::Builder b_excess_point_left;
        true_t::Builder b_excess_point_right;
    };

private:
    GetQueryResponse(Nat&& query_fields_mask,
                     Nat&& fields_mask,
                     series&& series,
                     array<seriesMeta>&& series_meta,
                     array<Int>&& chunk_ids,
                     Int&& total_time_points,
                     Long&& response_id,
                     std::optional<true_t>&& excess_point_left,
                     std::optional<true_t>&& excess_point_right) noexcept
        : m_query_fields_mask(std::move(query_fields_mask))
        , m_fields_mask(std::move(fields_mask))
        , m_series(std::move(series))
        , m_series_meta(std::move(series_meta))
        , m_chunk_ids(std::move(chunk_ids))
        , m_total_time_points(std::move(total_time_points))
        , m_response_id(std::move(response_id))
        , m_excess_point_left(std::move(excess_point_left))
        , m_excess_point_right(std::move(excess_point_right))
    {}

    Nat m_query_fields_mask;
    Nat m_fields_mask;
    series m_series;
    array<seriesMeta> m_series_meta;
    array<Int> m_chunk_ids;
    Int m_total_time_points;
    Long m_response_id;
    std::optional<true_t> m_excess_point_left;
    std::optional<true_t> m_excess_point_right;
};

bool operator==(const GetQueryResponse& lhs, const GetQueryResponse& rhs) noexcept
{
    return lhs.get_query_fields_mask() == rhs.get_query_fields_mask()
           && lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_series() == rhs.get_series()
           && lhs.get_series_meta() == rhs.get_series_meta()
           && lhs.get_chunk_ids() == rhs.get_chunk_ids()
           && lhs.get_total_time_points() == rhs.get_total_time_points()
           && lhs.get_response_id() == rhs.get_response_id()
           && lhs.get_excess_point_left() == rhs.get_excess_point_left()
           && lhs.get_excess_point_right() == rhs.get_excess_point_right();
}

}    // namespace baseline::statshouse
