#pragma once

#include "prelude/baseline/Includes.hpp"

#include "samples/baseline/statshouse/True.hpp"
#include "samples/baseline/statshouse/api/Filter.hpp"
#include "samples/baseline/statshouse/api/Function.hpp"



namespace baseline::statshouse {

class Query {
public:
    static constexpr Magic MAGIC = 3001253443;

    Query() noexcept = default;

    Query(const Query&) noexcept = default;
    Query(Query&&) noexcept = default;
    Query& operator=(const Query&) noexcept = default;
    Query& operator=(Query&&) noexcept = default;

    ~Query() noexcept = default;

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

    const std::optional<string>& get_promql() const noexcept
    {
        return m_promql;
    }

    const std::optional<array<Function>>& get_what() const noexcept
    {
        return m_what;
    }

    const std::optional<true_t>& get_excess_points_flag() const noexcept
    {
        return m_excess_points_flag;
    }

    const std::optional<string>& get_widthAgg() const noexcept
    {
        return m_widthAgg;
    }

    const std::optional<true_t>& get_name_flag() const noexcept
    {
        return m_name_flag;
    }

    const std::optional<true_t>& get_color_flag() const noexcept
    {
        return m_color_flag;
    }

    const std::optional<true_t>& get_total_flag() const noexcept
    {
        return m_total_flag;
    }

    const std::optional<true_t>& get_max_host_flag() const noexcept
    {
        return m_max_host_flag;
    }

    static Query fetch(InputStream& stream)
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
        std::optional<string> promql;
        if (IS_SET(fields_mask, 0)) promql = string::fetch(stream);
        std::optional<array<Function>> what;
        if (IS_SET(fields_mask, 1)) what = array<Function>::fetch(stream);
        std::optional<true_t> excess_points_flag;
        if (IS_SET(fields_mask, 2)) excess_points_flag = true_t::fetch(stream);
        std::optional<string> widthAgg;
        if (IS_SET(fields_mask, 3)) widthAgg = string::fetch(stream);
        std::optional<true_t> name_flag;
        if (IS_SET(fields_mask, 4)) name_flag = true_t::fetch(stream);
        std::optional<true_t> color_flag;
        if (IS_SET(fields_mask, 5)) color_flag = true_t::fetch(stream);
        std::optional<true_t> total_flag;
        if (IS_SET(fields_mask, 6)) total_flag = true_t::fetch(stream);
        std::optional<true_t> max_host_flag;
        if (IS_SET(fields_mask, 7)) max_host_flag = true_t::fetch(stream);
        Query result(std::move(fields_mask),
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
                     std::move(promql),
                     std::move(what),
                     std::move(excess_points_flag),
                     std::move(widthAgg),
                     std::move(name_flag),
                     std::move(color_flag),
                     std::move(total_flag),
                     std::move(max_host_flag));
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
        if (IS_SET(get_fields_mask(), 0)) m_promql->store(stream);
        if (IS_SET(get_fields_mask(), 1)) m_what->store(stream);
        if (IS_SET(get_fields_mask(), 2)) m_excess_points_flag->store(stream);
        if (IS_SET(get_fields_mask(), 3)) m_widthAgg->store(stream);
        if (IS_SET(get_fields_mask(), 4)) m_name_flag->store(stream);
        if (IS_SET(get_fields_mask(), 5)) m_color_flag->store(stream);
        if (IS_SET(get_fields_mask(), 6)) m_total_flag->store(stream);
        if (IS_SET(get_fields_mask(), 7)) m_max_host_flag->store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const Query& rhs) noexcept
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
                   && (!rhs.get_promql() || lhs.b_promql == rhs.get_promql())
                   && (!rhs.get_what() || lhs.b_what == rhs.get_what())
                   && (!rhs.get_excess_points_flag() || lhs.b_excess_points_flag == rhs.get_excess_points_flag())
                   && (!rhs.get_widthAgg() || lhs.b_widthAgg == rhs.get_widthAgg())
                   && (!rhs.get_name_flag() || lhs.b_name_flag == rhs.get_name_flag())
                   && (!rhs.get_color_flag() || lhs.b_color_flag == rhs.get_color_flag())
                   && (!rhs.get_total_flag() || lhs.b_total_flag == rhs.get_total_flag())
                   && (!rhs.get_max_host_flag() || lhs.b_max_host_flag == rhs.get_max_host_flag());
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3, size_t SIZE_4, size_t SIZE_5, size_t SIZE_6, size_t SIZE_7, size_t SIZE_8, size_t SIZE_9, size_t SIZE_10, size_t SIZE_11>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(utils::random_mask<0, 255>(engine))
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
                    .set_promql(string::Builder::random<SIZE_9>(engine))
                    .set_what(array<Function>::Builder::random<SIZE_10>(engine))
                    .set_excess_points_flag(true_t::Builder::random(engine))
                    .set_widthAgg(string::Builder::random<SIZE_11>(engine))
                    .set_name_flag(true_t::Builder::random(engine))
                    .set_color_flag(true_t::Builder::random(engine))
                    .set_total_flag(true_t::Builder::random(engine))
                    .set_max_host_flag(true_t::Builder::random(engine));
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

        Builder& set_promql(const string::Builder& value) noexcept
        {
            b_promql = value;
            return *this;
        }

        Builder& set_what(const array<Function>::Builder& value) noexcept
        {
            b_what = value;
            return *this;
        }

        Builder& set_excess_points_flag(const true_t::Builder& value) noexcept
        {
            b_excess_points_flag = value;
            return *this;
        }

        Builder& set_widthAgg(const string::Builder& value) noexcept
        {
            b_widthAgg = value;
            return *this;
        }

        Builder& set_name_flag(const true_t::Builder& value) noexcept
        {
            b_name_flag = value;
            return *this;
        }

        Builder& set_color_flag(const true_t::Builder& value) noexcept
        {
            b_color_flag = value;
            return *this;
        }

        Builder& set_total_flag(const true_t::Builder& value) noexcept
        {
            b_total_flag = value;
            return *this;
        }

        Builder& set_max_host_flag(const true_t::Builder& value) noexcept
        {
            b_max_host_flag = value;
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
            if (IS_SET(b_fields_mask, 0)) b_promql.store(stream);
            if (IS_SET(b_fields_mask, 1)) b_what.store(stream);
            if (IS_SET(b_fields_mask, 2)) b_excess_points_flag.store(stream);
            if (IS_SET(b_fields_mask, 3)) b_widthAgg.store(stream);
            if (IS_SET(b_fields_mask, 4)) b_name_flag.store(stream);
            if (IS_SET(b_fields_mask, 5)) b_color_flag.store(stream);
            if (IS_SET(b_fields_mask, 6)) b_total_flag.store(stream);
            if (IS_SET(b_fields_mask, 7)) b_max_host_flag.store(stream);
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
        string::Builder b_promql;
        array<Function>::Builder b_what;
        true_t::Builder b_excess_points_flag;
        string::Builder b_widthAgg;
        true_t::Builder b_name_flag;
        true_t::Builder b_color_flag;
        true_t::Builder b_total_flag;
        true_t::Builder b_max_host_flag;
    };

private:
    Query(Nat&& fields_mask,
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
          std::optional<string>&& promql,
          std::optional<array<Function>>&& what,
          std::optional<true_t>&& excess_points_flag,
          std::optional<string>&& widthAgg,
          std::optional<true_t>&& name_flag,
          std::optional<true_t>&& color_flag,
          std::optional<true_t>&& total_flag,
          std::optional<true_t>&& max_host_flag) noexcept
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
        , m_promql(std::move(promql))
        , m_what(std::move(what))
        , m_excess_points_flag(std::move(excess_points_flag))
        , m_widthAgg(std::move(widthAgg))
        , m_name_flag(std::move(name_flag))
        , m_color_flag(std::move(color_flag))
        , m_total_flag(std::move(total_flag))
        , m_max_host_flag(std::move(max_host_flag))
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
    std::optional<string> m_promql;
    std::optional<array<Function>> m_what;
    std::optional<true_t> m_excess_points_flag;
    std::optional<string> m_widthAgg;
    std::optional<true_t> m_name_flag;
    std::optional<true_t> m_color_flag;
    std::optional<true_t> m_total_flag;
    std::optional<true_t> m_max_host_flag;
};

bool operator==(const Query& lhs, const Query& rhs) noexcept
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
           && lhs.get_promql() == rhs.get_promql()
           && lhs.get_what() == rhs.get_what()
           && lhs.get_excess_points_flag() == rhs.get_excess_points_flag()
           && lhs.get_widthAgg() == rhs.get_widthAgg()
           && lhs.get_name_flag() == rhs.get_name_flag()
           && lhs.get_color_flag() == rhs.get_color_flag()
           && lhs.get_total_flag() == rhs.get_total_flag()
           && lhs.get_max_host_flag() == rhs.get_max_host_flag();
}

}    // namespace baseline::statshouse
