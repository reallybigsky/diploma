#pragma once

#include "baseline/Includes.hpp"
#include "baseline/statshouse/api/Series.hpp"
#include "baseline/statshouse/api/SeriesMeta.hpp"



namespace baseline::statshouse {

//template <Nat...>
//class GetQueryResponse;
//
//template <>
//class GetQueryResponse<> {
class GetQueryResponse {
public:
    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
    static constexpr Magic MAGIC = 1148016255;
    static constexpr bool CTS = false;

    GetQueryResponse() noexcept = default;

    GetQueryResponse(const GetQueryResponse&) noexcept = default;
    GetQueryResponse(GetQueryResponse&&) noexcept = default;
    GetQueryResponse& operator=(const GetQueryResponse&) noexcept = default;
    GetQueryResponse& operator=(GetQueryResponse&&) noexcept = default;

    ~GetQueryResponse() noexcept = default;

    GetQueryResponse(Nat query_fields_mask,
                     Nat fields_mask,
                     const Series& series,
                     const SeriesMeta<>& series_meta,
                     const Array<Int>& chunk_ids,
                     Int total_time_points,
                     Long response_id,
                     Nat excess_point_left,
                     Nat excess_point_right) noexcept
        : m_query_fields_mask(query_fields_mask)
        , m_fields_mask(fields_mask)
        , m_series(series)
        , m_series_meta(series_meta)
        , m_chunk_ids(chunk_ids)
        , m_total_time_points(total_time_points)
        , m_response_id(response_id)
        , m_excess_point_left(excess_point_left)
        , m_excess_point_right(excess_point_right)
    {}

    Nat get_query_fields_mask() const noexcept
    {
        return m_query_fields_mask;
    }

    Nat get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const Series& get_series() const noexcept
    {
        return m_series;
    }

    const SeriesMeta<>& get_series_meta() const noexcept
    {
        return m_series_meta;
    }

    const Array<Int>& get_chunk_ids() const noexcept
    {
        return m_chunk_ids;
    }

    Int get_total_time_points() const noexcept
    {
        return m_total_time_points;
    }

    Long get_response_id() const noexcept
    {
        return m_response_id;
    }

    Nat get_excess_point_left() const noexcept
    {
        return m_excess_point_left;
    }

    Nat get_excess_point_right() const noexcept
    {
        return m_excess_point_right;
    }

    static GetQueryResponse fetch(InputStream& stream)
    {
        // HighLevel => fetch magic in higher variant
        //
        // Magic magic = Magic::fetch(stream);
        // if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);

        Nat query_fields_mask = Nat::fetch(stream);
        Nat fields_mask = Nat::fetch(stream);
        Series series = Series::fetch(stream);
        SeriesMeta<> series_meta = SeriesMeta<>::fetch(query_fields_mask, stream);
        Array<Int> chunk_ids = Array<Int>::fetch(stream);
        Int total_time_points = Int::fetch(stream);
        Long response_id = Long::fetch(stream);
        Nat excess_point_left;
        if (IS_SET(fields_mask, 0)) excess_point_left = Nat::fetch(stream);
        Nat excess_point_right;
        if (IS_SET(fields_mask, 1)) excess_point_right = Nat::fetch(stream);
        return {query_fields_mask,
                fields_mask,
                series,
                series_meta,
                chunk_ids,
                total_time_points,
                response_id,
                excess_point_left,
                excess_point_right};
    }

    void store(OutputStream& stream) const
    {
        MAGIC.store(stream);
        m_query_fields_mask.store(stream);
        m_fields_mask.store(stream);
        m_series.store(stream);
        m_series_meta.store(m_query_fields_mask, stream);
        m_chunk_ids.store(stream);
        m_total_time_points.store(stream);
        m_response_id.store(stream);
        if (IS_SET(m_fields_mask, 0)) m_excess_point_left.store(stream);
        if (IS_SET(m_fields_mask, 1)) m_excess_point_right.store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const GetQueryResponse& rhs)
        {
            return lhs.b_query_fields_mask == rhs.get_query_fields_mask()
                   && lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_series == rhs.get_series()
                   && lhs.b_series_meta == rhs.get_series_meta()
                   && lhs.b_chunk_ids == rhs.get_chunk_ids()
                   && lhs.b_total_time_points == rhs.get_total_time_points()
                   && lhs.b_response_id == rhs.get_response_id()
                   && (!IS_SET(lhs.b_fields_mask, 0) || lhs.b_excess_point_left == rhs.get_excess_point_left())
                   && (!IS_SET(lhs.b_fields_mask, 1) || lhs.b_excess_point_right == rhs.get_excess_point_right());
        }

        Builder& set_query_fields_mask(Nat::Builder value) noexcept
        {
            b_query_fields_mask = value;
            return *this;
        }

        Builder& set_fields_mask(Nat::Builder value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_series(const Series::Builder& value) noexcept
        {
            b_series = value;
            return *this;
        }

        Builder& set_series_meta(const SeriesMeta<>::Builder& value) noexcept
        {
            b_series_meta = value;
            return *this;
        }

        Builder& set_chunk_ids(const Array<Int>::Builder& value) noexcept
        {
            b_chunk_ids = value;
            return *this;
        }

        Builder& set_total_time_points(Int::Builder value) noexcept
        {
            b_total_time_points = value;
            return *this;
        }

        Builder& set_response_id(Long::Builder value) noexcept
        {
            b_response_id = value;
            return *this;
        }

        Builder& set_excess_point_left(Nat::Builder value) noexcept
        {
            b_excess_point_left = value;
            return *this;
        }

        Builder& set_excess_point_right(Nat::Builder value) noexcept
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
            b_series_meta.store(b_query_fields_mask, stream);
            b_chunk_ids.store(stream);
            b_total_time_points.store(stream);
            b_response_id.store(stream);
            if (IS_SET(b_fields_mask, 0)) b_excess_point_left.store(stream);
            if (IS_SET(b_fields_mask, 1)) b_excess_point_right.store(stream);
        }

    private:
        Nat::Builder b_query_fields_mask;
        Nat::Builder b_fields_mask;
        Series::Builder b_series;
        SeriesMeta<>::Builder b_series_meta;
        Array<Int>::Builder b_chunk_ids;
        Int::Builder b_total_time_points;
        Long::Builder b_response_id;
        Nat::Builder b_excess_point_left;
        Nat::Builder b_excess_point_right;
    };

private:
    Nat m_query_fields_mask;
    Nat m_fields_mask;
    Series m_series;
    SeriesMeta<> m_series_meta;
    Array<Int> m_chunk_ids;
    Int m_total_time_points;
    Long m_response_id;
    Nat m_excess_point_left;
    Nat m_excess_point_right;
};

//template <Nat m_QUERY_FIELDS_MASK>
//class GetQueryResponse<m_QUERY_FIELDS_MASK> {
//public:
//    static constexpr Nat QUERY_FIELDS_MASK = m_QUERY_FIELDS_MASK;
//    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
//    static constexpr Magic MAGIC = 1148016255;
//    static constexpr bool CTS = false;
//
//    GetQueryResponse() noexcept = default;
//
//    GetQueryResponse(const GetQueryResponse&) noexcept = default;
//    GetQueryResponse(GetQueryResponse&&) noexcept = default;
//    GetQueryResponse& operator=(const GetQueryResponse&) noexcept = default;
//    GetQueryResponse& operator=(GetQueryResponse&&) noexcept = default;
//
//    ~GetQueryResponse() noexcept = default;
//
//    GetQueryResponse(Nat fields_mask,
//                     const Series& series,
//                     const SeriesMeta<m_QUERY_FIELDS_MASK>& series_meta,
//                     const Array<Int>& chunk_ids,
//                     Int total_time_points,
//                     Long response_id,
//                     Nat excess_point_left,
//                     Nat excess_point_right) noexcept
//        : m_fields_mask(fields_mask)
//        , m_series(series)
//        , m_series_meta(series_meta)
//        , m_chunk_ids(chunk_ids)
//        , m_total_time_points(total_time_points)
//        , m_response_id(response_id)
//        , m_excess_point_left(excess_point_left)
//        , m_excess_point_right(excess_point_right)
//    {}
//
//    Nat get_fields_mask() const noexcept
//    {
//        return m_fields_mask;
//    }
//
//    const Series& get_series() const noexcept
//    {
//        return m_series;
//    }
//
//    const SeriesMeta<m_QUERY_FIELDS_MASK>& get_series_meta() const noexcept
//    {
//        return m_series_meta;
//    }
//
//    const Array<Int>& get_chunk_ids() const noexcept
//    {
//        return m_chunk_ids;
//    }
//
//    Int get_total_time_points() const noexcept
//    {
//        return m_total_time_points;
//    }
//
//    Long get_response_id() const noexcept
//    {
//        return m_response_id;
//    }
//
//    Nat get_excess_point_left() const noexcept
//    {
//        return m_excess_point_left;
//    }
//
//    Nat get_excess_point_right() const noexcept
//    {
//        return m_excess_point_right;
//    }
//
//    static GetQueryResponse fetch(InputStream& stream)
//    {
//        Magic magic = Magic::fetch(stream);
//        if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
//
//        Nat fields_mask = Nat::fetch(stream);
//        Series series = Series::fetch(stream);
//        SeriesMeta<m_QUERY_FIELDS_MASK> series_meta = SeriesMeta<m_QUERY_FIELDS_MASK>::fetch(stream);
//        Array<Int> chunk_ids = Array<Int>::fetch(stream);
//        Int total_time_points = Int::fetch(stream);
//        Long response_id = Long::fetch(stream);
//        Nat excess_point_left;
//        if (IS_SET(fields_mask, 0)) excess_point_left = Nat::fetch(stream);
//        Nat excess_point_right;
//        if (IS_SET(fields_mask, 1)) excess_point_right = Nat::fetch(stream);
//        return {fields_mask,
//                series,
//                series_meta,
//                chunk_ids,
//                total_time_points,
//                response_id,
//                excess_point_left,
//                excess_point_right};
//    }
//
//    void store(OutputStream& stream) const
//    {
//        MAGIC.store(stream);
//        m_fields_mask.store(stream);
//        m_series.store(stream);
//        m_series_meta.store(stream);
//        m_chunk_ids.store(stream);
//        m_total_time_points.store(stream);
//        m_response_id.store(stream);
//        if (IS_SET(m_fields_mask, 0)) m_excess_point_left.store(stream);
//        if (IS_SET(m_fields_mask, 1)) m_excess_point_right.store(stream);
//    }
//
//    class Builder {
//    public:
//        Builder& set_fields_mask(Nat::Builder value) noexcept
//        {
//            b_fields_mask = value;
//            return *this;
//        }
//
//        Builder& set_series(const Series::Builder& value) noexcept
//        {
//            b_series = value;
//            return *this;
//        }
//
//        Builder& set_series_meta(const SeriesMeta<m_QUERY_FIELDS_MASK>::Builder& value) noexcept
//        {
//            b_series_meta = value;
//            return *this;
//        }
//
//        Builder& set_chunk_ids(const Array<Int>::Builder& value) noexcept
//        {
//            b_chunk_ids = value;
//            return *this;
//        }
//
//        Builder& set_total_time_points(Int::Builder value) noexcept
//        {
//            b_total_time_points = value;
//            return *this;
//        }
//
//        Builder& set_response_id(Long::Builder value) noexcept
//        {
//            b_response_id = value;
//            return *this;
//        }
//
//        Builder& set_excess_point_left(Nat::Builder value) noexcept
//        {
//            b_excess_point_left = value;
//            return *this;
//        }
//
//        Builder& set_excess_point_right(Nat::Builder value) noexcept
//        {
//            b_excess_point_right = value;
//            return *this;
//        }
//
//        void store(OutputStream& stream) const
//        {
//            MAGIC.store(stream);
//            b_fields_mask.store(stream);
//            b_series.store(stream);
//            b_series_meta.store(stream);
//            b_chunk_ids.store(stream);
//            b_total_time_points.store(stream);
//            b_response_id.store(stream);
//            if (IS_SET(b_fields_mask, 0)) b_excess_point_left.store(stream);
//            if (IS_SET(b_fields_mask, 1)) b_excess_point_right.store(stream);
//        }
//
//    private:
//        Nat::Builder b_fields_mask;
//        Series::Builder b_series;
//        SeriesMeta<m_QUERY_FIELDS_MASK>::Builder b_series_meta;
//        Array<Int>::Builder b_chunk_ids;
//        Int::Builder b_total_time_points;
//        Long::Builder b_response_id;
//        Nat::Builder b_excess_point_left;
//        Nat::Builder b_excess_point_right;
//    };
//
//private:
//    Nat m_fields_mask;
//    Series m_series;
//    SeriesMeta<m_QUERY_FIELDS_MASK> m_series_meta;
//    Array<Int> m_chunk_ids;
//    Int m_total_time_points;
//    Long m_response_id;
//    Nat m_excess_point_left;
//    Nat m_excess_point_right;
//};

}    // namespace baseline::statshouse
