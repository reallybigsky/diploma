#pragma once

#include "prelude/baseline/Includes.hpp"

#include "samples/baseline/statshouse/True.hpp"
#include "samples/baseline/statshouse/api/Filter.hpp"
#include "samples/baseline/statshouse/api/Function.hpp"



namespace baseline::statshouse {

class QueryPoint {
public:
    static constexpr Magic MAGIC = 31023132;

    QueryPoint() noexcept = default;

    QueryPoint(const QueryPoint&) noexcept = default;
    QueryPoint(QueryPoint&&) noexcept = default;
    QueryPoint& operator=(const QueryPoint&) noexcept = default;
    QueryPoint& operator=(QueryPoint&&) noexcept = default;

    ~QueryPoint() noexcept = default;

    const Nat& get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const Int& get_version() const noexcept
    {
        return m_version;
    }

    const Int& get_top_n() const noexcept
    {
        return m_top_n;
    }

    const string& get_metric_name() const noexcept
    {
        return m_metric_name;
    }

    const Long& get_time_from() const noexcept
    {
        return m_time_from;
    }

    const Long& get_time_to() const noexcept
    {
        return m_time_to;
    }

    const string& get_interval() const noexcept
    {
        return m_interval;
    }

    const Function& get_function() const noexcept
    {
        return m_function;
    }

    const array<string>& get_group_by() const noexcept
    {
        return m_group_by;
    }

    const array<filter>& get_filters() const noexcept
    {
        return m_filters;
    }

    const array<Long>& get_time_shift() const noexcept
    {
        return m_time_shift;
    }

    const std::optional<array<Function>>& get_what() const noexcept
    {
        return m_what;
    }

    static QueryPoint fetch(InputStream& stream)
    {
        Nat fields_mask = Nat::fetch(stream);
        Int version = Int::fetch(stream);
        Int top_n = Int::fetch(stream);
        string metric_name = string::fetch(stream);
        Long time_from = Long::fetch(stream);
        Long time_to = Long::fetch(stream);
        string interval = string::fetch(stream);
        Function function = Function::fetch(stream);
        array<string> group_by = array<string>::fetch(stream);
        array<filter> filters = array<filter>::fetch(stream);
        array<Long> time_shift = array<Long>::fetch(stream);
        std::optional<array<Function>> what;
        if (IS_SET(fields_mask, 1)) what = array<Function>::fetch(stream);
        QueryPoint result(std::move(fields_mask),
                          std::move(version),
                          std::move(top_n),
                          std::move(metric_name),
                          std::move(time_from),
                          std::move(time_to),
                          std::move(interval),
                          std::move(function),
                          std::move(group_by),
                          std::move(filters),
                          std::move(time_shift),
                          std::move(what));
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_fields_mask.store(stream);
        m_version.store(stream);
        m_top_n.store(stream);
        m_metric_name.store(stream);
        m_time_from.store(stream);
        m_time_to.store(stream);
        m_interval.store(stream);
        m_function.store(stream);
        m_group_by.store(stream);
        m_filters.store(stream);
        m_time_shift.store(stream);
        if (IS_SET(get_fields_mask(), 1)) m_what->store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const QueryPoint& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_version == rhs.get_version()
                   && lhs.b_top_n == rhs.get_top_n()
                   && lhs.b_metric_name == rhs.get_metric_name()
                   && lhs.b_time_from == rhs.get_time_from()
                   && lhs.b_time_to == rhs.get_time_to()
                   && lhs.b_interval == rhs.get_interval()
                   && lhs.b_function == rhs.get_function()
                   && lhs.b_group_by == rhs.get_group_by()
                   && lhs.b_filters == rhs.get_filters()
                   && lhs.b_time_shift == rhs.get_time_shift()
                   && (!rhs.get_what() || lhs.b_what == rhs.get_what());
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3, size_t SIZE_4, size_t SIZE_5, size_t SIZE_6, size_t SIZE_7, size_t SIZE_8, size_t SIZE_9, size_t SIZE_10, size_t SIZE_11>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(utils::random_mask<0, 3>(engine))
                    .set_version(Int::Builder::random(engine))
                    .set_top_n(Int::Builder::random(engine))
                    .set_metric_name(string::Builder::random<SIZE_1>(engine))
                    .set_time_from(Long::Builder::random(engine))
                    .set_time_to(Long::Builder::random(engine))
                    .set_interval(string::Builder::random<SIZE_2>(engine))
                    .set_function(Function::Builder::random(engine))
                    .set_group_by(array<string>::Builder::random<SIZE_3, SIZE_4>(engine))
                    .set_filters(array<filter>::Builder::random<SIZE_5, SIZE_6, SIZE_6>(engine))
                    .set_time_shift(array<Long>::Builder::random<SIZE_8>(engine))
                    .set_what(array<Function>::Builder::random<SIZE_10>(engine));
        }

        Builder& set_fields_mask(const Nat::Builder& value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_version(const Int::Builder& value) noexcept
        {
            b_version = value;
            return *this;
        }

        Builder& set_top_n(const Int::Builder& value) noexcept
        {
            b_top_n = value;
            return *this;
        }

        Builder& set_metric_name(const string::Builder& value) noexcept
        {
            b_metric_name = value;
            return *this;
        }

        Builder& set_time_from(const Long::Builder& value) noexcept
        {
            b_time_from = value;
            return *this;
        }

        Builder& set_time_to(const Long::Builder& value) noexcept
        {
            b_time_to = value;
            return *this;
        }

        Builder& set_interval(const string::Builder& value) noexcept
        {
            b_interval = value;
            return *this;
        }

        Builder& set_function(const Function::Builder& value) noexcept
        {
            b_function = value;
            return *this;
        }

        Builder& set_group_by(const array<string>::Builder& value) noexcept
        {
            b_group_by = value;
            return *this;
        }

        Builder& set_filters(const array<filter>::Builder& value) noexcept
        {
            b_filters = value;
            return *this;
        }

        Builder& set_time_shift(const array<Long>::Builder& value) noexcept
        {
            b_time_shift = value;
            return *this;
        }

        Builder& set_what(const array<Function>::Builder& value) noexcept
        {
            b_what = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            MAGIC.store(stream);
            b_fields_mask.store(stream);
            b_version.store(stream);
            b_top_n.store(stream);
            b_metric_name.store(stream);
            b_time_from.store(stream);
            b_time_to.store(stream);
            b_interval.store(stream);
            b_function.store(stream);
            b_group_by.store(stream);
            b_filters.store(stream);
            b_time_shift.store(stream);
            if (IS_SET(b_fields_mask, 1)) b_what.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        Int::Builder b_version;
        Int::Builder b_top_n;
        string::Builder b_metric_name;
        Long::Builder b_time_from;
        Long::Builder b_time_to;
        string::Builder b_interval;
        Function::Builder b_function;
        array<string>::Builder b_group_by;
        array<filter>::Builder b_filters;
        array<Long>::Builder b_time_shift;
        array<Function>::Builder b_what;
    };

private:
    QueryPoint(Nat&& fields_mask,
               Int&& version,
               Int&& top_n,
               string&& metric_name,
               Long&& time_from,
               Long&& time_to,
               string&& interval,
               Function&& function,
               array<string>&& group_by,
               array<filter>&& filters,
               array<Long>&& time_shift,
               std::optional<array<Function>>&& what) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_version(std::move(version))
        , m_top_n(std::move(top_n))
        , m_metric_name(std::move(metric_name))
        , m_time_from(std::move(time_from))
        , m_time_to(std::move(time_to))
        , m_interval(std::move(interval))
        , m_function(std::move(function))
        , m_group_by(std::move(group_by))
        , m_filters(std::move(filters))
        , m_time_shift(std::move(time_shift))
        , m_what(std::move(what))
    {}

    Nat m_fields_mask;
    Int m_version;
    Int m_top_n;
    string m_metric_name;
    Long m_time_from;
    Long m_time_to;
    string m_interval;
    Function m_function;
    array<string> m_group_by;
    array<filter> m_filters;
    array<Long> m_time_shift;
    std::optional<array<Function>> m_what;
};

bool operator==(const QueryPoint& lhs, const QueryPoint& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_version() == rhs.get_version()
           && lhs.get_top_n() == rhs.get_top_n()
           && lhs.get_metric_name() == rhs.get_metric_name()
           && lhs.get_time_from() == rhs.get_time_from()
           && lhs.get_time_to() == rhs.get_time_to()
           && lhs.get_interval() == rhs.get_interval()
           && lhs.get_function() == rhs.get_function()
           && lhs.get_group_by() == rhs.get_group_by()
           && lhs.get_filters() == rhs.get_filters()
           && lhs.get_time_shift() == rhs.get_time_shift()
           && lhs.get_what() == rhs.get_what();
}

}    // namespace baseline::statshouse
