#pragma once

#include "baseline/Includes.hpp"



namespace baseline::statshouse {

template <Nat... MASK>
class SeriesMeta;

template <>
class SeriesMeta<> {
public:
    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
    static constexpr Magic MAGIC = 3280544510;
    static constexpr bool CTS = false;

    SeriesMeta() noexcept = default;

    SeriesMeta(const SeriesMeta&) noexcept = default;
    SeriesMeta(SeriesMeta&&) noexcept = default;
    SeriesMeta& operator=(const SeriesMeta&) noexcept = default;
    SeriesMeta& operator=(SeriesMeta&&) noexcept = default;

    ~SeriesMeta() noexcept = default;

    SeriesMeta(Nat fields_mask,
               Long time_shift,
               const Dictionary<string>& tags,
               Nat what,
               const string& name,
               const string& color,
               Int total,
               const Array<string>& max_hosts) noexcept
        : m_fields_mask(fields_mask)
        , m_time_shift(time_shift)
        , m_tags(tags)
        , m_what(what)
        , m_name(name)
        , m_color(color)
        , m_total(total)
        , m_max_hosts(max_hosts)
    {}

    Nat get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    Long get_time_shift() const noexcept
    {
        return m_time_shift;
    }

    const Dictionary<string>& get_tags() const noexcept
    {
        return m_tags;
    }

    Nat get_what() const noexcept
    {
        return m_what;
    }

    const string& get_name() const noexcept
    {
        return m_name;
    }

    const string& get_color() const noexcept
    {
        return m_color;
    }

    Int get_total() const noexcept
    {
        return m_total;
    }

    const Array<string>& get_max_hosts() const noexcept
    {
        return m_max_hosts;
    }

    static SeriesMeta fetch(Nat query_fields_mask, InputStream& stream)
    {
        Magic magic = Magic::fetch(stream);
        if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);

        Nat fields_mask = Nat::fetch(stream);
        Long time_shift = Long::fetch(stream);
        Dictionary<string> tags = Dictionary<string>::fetch(stream);
        Nat what;
        if (IS_SET(fields_mask, 1)) what = Nat::fetch(stream);
        string name;
        if (IS_SET(query_fields_mask, 4)) name = string::fetch(stream);
        string color;
        if (IS_SET(query_fields_mask, 5)) color = string::fetch(stream);
        Int total;
        if (IS_SET(query_fields_mask, 6)) total = Int::fetch(stream);
        Array<string> max_hosts;
        if (IS_SET(query_fields_mask, 7)) max_hosts = Array<string>::fetch(stream);
        return {fields_mask,
                time_shift,
                tags,
                what,
                name,
                color,
                total,
                max_hosts};
    }

    void store(Nat query_fields_mask, OutputStream& stream) const
    {
        MAGIC.store(stream);
        m_fields_mask.store(stream);
        m_time_shift.store(stream);
        m_tags.store(stream);
        if (IS_SET(m_fields_mask, 1)) m_what.store(stream);
        if (IS_SET(query_fields_mask, 4)) m_name.store(stream);
        if (IS_SET(query_fields_mask, 5)) m_color.store(stream);
        if (IS_SET(query_fields_mask, 6)) m_total.store(stream);
        if (IS_SET(query_fields_mask, 7)) m_max_hosts.store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const SeriesMeta<>& rhs)
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_time_shift == rhs.get_time_shift()
                   && lhs.b_tags == rhs.get_tags()
                   && (!IS_SET(lhs.b_fields_mask, 1) || lhs.b_what == rhs.get_what())
                   && lhs.b_name == rhs.get_name()
                   && lhs.b_color == rhs.get_color()
                   && lhs.b_total == rhs.get_total()
                   && lhs.b_max_hosts == rhs.get_max_hosts();
        }

        Builder& set_fields_mask(Nat::Builder value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_time_shift(Long::Builder value) noexcept
        {
            b_time_shift = value;
            return *this;
        }

        Builder& set_tags(const Dictionary<string>::Builder& value) noexcept
        {
            b_tags = value;
            return *this;
        }

        Builder& set_what(Nat::Builder value) noexcept
        {
            b_what = value;
            return *this;
        }

        Builder& set_name(const string::Builder& value) noexcept
        {
            b_name = value;
            return *this;
        }

        Builder& set_color(const string::Builder& value) noexcept
        {
            b_color = value;
            return *this;
        }

        Builder& set_total(Int::Builder value) noexcept
        {
            b_total = value;
            return *this;
        }

        Builder& set_max_hosts(const Array<string>::Builder& value) noexcept
        {
            b_max_hosts = value;
            return *this;
        }

        void store(Nat::Builder query_fields_mask, OutputStream& stream) const
        {
            MAGIC.store(stream);
            b_fields_mask.store(stream);
            b_time_shift.store(stream);
            b_tags.store(stream);
            if (IS_SET(b_fields_mask, 1)) b_what.store(stream);
            if (IS_SET(query_fields_mask, 4)) b_name.store(stream);
            if (IS_SET(query_fields_mask, 5)) b_color.store(stream);
            if (IS_SET(query_fields_mask, 6)) b_total.store(stream);
            if (IS_SET(query_fields_mask, 7)) b_max_hosts.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        Long::Builder b_time_shift;
        Dictionary<string>::Builder b_tags;
        Nat::Builder b_what;
        string::Builder b_name;
        string::Builder b_color;
        Int::Builder b_total;
        Array<string>::Builder b_max_hosts;
    };

private:
    Nat m_fields_mask;
    Long m_time_shift;
    Dictionary<string> m_tags;
    Nat m_what;
    string m_name;
    string m_color;
    Int m_total;
    Array<string> m_max_hosts;
};

template <Nat m_QUERY_FIELDS_MASK>
class SeriesMeta<m_QUERY_FIELDS_MASK>
    : private MAYBE<string, IS_SET(m_QUERY_FIELDS_MASK, 4), 0>
    , private MAYBE<string, IS_SET(m_QUERY_FIELDS_MASK, 5), 1>
    , private MAYBE<Int, IS_SET(m_QUERY_FIELDS_MASK, 6), 2>
    , private MAYBE<Array<string>, IS_SET(m_QUERY_FIELDS_MASK, 7), 3> {
private:
    using t_name = MAYBE<string, IS_SET(m_QUERY_FIELDS_MASK, 4), 0>;
    using t_color = MAYBE<string, IS_SET(m_QUERY_FIELDS_MASK, 5), 1>;
    using t_total = MAYBE<Int, IS_SET(m_QUERY_FIELDS_MASK, 6), 2>;
    using t_max_hosts = MAYBE<Array<string>, IS_SET(m_QUERY_FIELDS_MASK, 7), 3>;

public:
    static constexpr Nat QUERY_FIELDS_MASK = m_QUERY_FIELDS_MASK;
    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
    static constexpr Magic MAGIC = 3280544510;
    static constexpr bool CTS = false;

    SeriesMeta() noexcept = default;

    SeriesMeta(const SeriesMeta&) noexcept = default;
    SeriesMeta(SeriesMeta&&) noexcept = default;
    SeriesMeta& operator=(const SeriesMeta&) noexcept = default;
    SeriesMeta& operator=(SeriesMeta&&) noexcept = default;

    ~SeriesMeta() noexcept = default;

    SeriesMeta(Nat fields_mask,
               Long time_shift,
               const Dictionary<string>& tags,
               Nat what,
               const string& name,
               const string& color,
               Int total,
               const Array<string>& max_hosts) noexcept
        : m_fields_mask(fields_mask)
        , m_time_shift(time_shift)
        , m_tags(tags)
        , m_what(what)
        , t_name(name)
        , t_color(color)
        , t_total(total)
        , t_max_hosts(max_hosts)
    {}

    Nat get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    Long get_time_shift() const noexcept
    {
        return m_time_shift;
    }

    const Dictionary<string>& get_tags() const noexcept
    {
        return m_tags;
    }

    Nat get_what() const noexcept
    {
        return m_what;
    }

    const string& get_name() const noexcept
    requires(IS_SET(m_QUERY_FIELDS_MASK, 4))
    {
        return t_name::value;
    }

    const string& get_color() const noexcept
    requires(IS_SET(m_QUERY_FIELDS_MASK, 5))
    {
        return t_color::value;
    }

    Int get_total() const noexcept
    requires(IS_SET(m_QUERY_FIELDS_MASK, 6))
    {
        return t_total::value;
    }

    const Array<string>& get_max_hosts() const noexcept
    requires(IS_SET(m_QUERY_FIELDS_MASK, 7))
    {
        return t_max_hosts::value;
    }

    static SeriesMeta fetch(InputStream& stream)
    {
        Magic magic = Magic::fetch(stream);
        if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);

        Nat fields_mask = Nat::fetch(stream);
        Long time_shift = Long::fetch(stream);
        Dictionary<string> tags = Dictionary<string>::fetch(stream);
        Nat what;
        if (IS_SET(fields_mask, 1)) what = Nat::fetch(stream);
        string name;
        if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 4)) name = string::fetch(stream);
        string color;
        if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 5)) color = string::fetch(stream);
        Int total;
        if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 6)) total = Int::fetch(stream);
        Array<string> max_hosts;
        if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 7)) max_hosts = Array<string>::fetch(stream);
        return {fields_mask,
                time_shift,
                tags,
                what,
                name,
                color,
                total,
                max_hosts};
    }

    void store(OutputStream& stream) const
    {
        MAGIC.store(stream);
        m_fields_mask.store(stream);
        m_time_shift.store(stream);
        m_tags.store(stream);
        if (IS_SET(m_fields_mask, 1)) m_what.store(stream);
        if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 4)) t_name::value.store(stream);
        if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 5)) t_color::value.store(stream);
        if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 6)) t_total::value.store(stream);
        if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 7)) t_max_hosts::value.store(stream);
    }

    class Builder
        : private MAYBE<string::Builder, IS_SET(m_QUERY_FIELDS_MASK, 4), 0>
        , private MAYBE<string::Builder, IS_SET(m_QUERY_FIELDS_MASK, 5), 1>
        , private MAYBE<Int::Builder, IS_SET(m_QUERY_FIELDS_MASK, 6), 2>
        , private MAYBE<Array<string>::Builder, IS_SET(m_QUERY_FIELDS_MASK, 7), 3> {
    private:
        using b_name = MAYBE<string::Builder, IS_SET(m_QUERY_FIELDS_MASK, 4), 0>;
        using b_color = MAYBE<string::Builder, IS_SET(m_QUERY_FIELDS_MASK, 5), 1>;
        using b_total = MAYBE<Int::Builder, IS_SET(m_QUERY_FIELDS_MASK, 6), 2>;
        using b_max_hosts = MAYBE<Array<string>::Builder, IS_SET(m_QUERY_FIELDS_MASK, 7), 3>;

    public:
        friend bool operator==(const Builder& lhs, const SeriesMeta<m_QUERY_FIELDS_MASK>& rhs)
        {
            if (lhs.b_fields_mask != rhs.get_fields_mask())
                return false;

            if (lhs.b_time_shift != rhs.get_time_shift())
                return false;

            if (lhs.b_tags != rhs.get_tags())
                return false;

            if (IS_SET(lhs.b_fields_mask, 1) && lhs.b_what != rhs.get_what())
                return false;

            if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 4))
                if (lhs.b_name::value != rhs.get_name())
                    return false;

            if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 5))
                if (lhs.b_color::value != rhs.get_color())
                    return false;

            if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 6))
                if (lhs.b_total::value != rhs.get_total())
                    return false;

            if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 7))
                if (lhs.b_max_hosts::value != rhs.get_max_hosts())
                    return false;

            return true;
        }

        Builder& set_fields_mask(Nat::Builder value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_time_shift(Long::Builder value) noexcept
        {
            b_time_shift = value;
            return *this;
        }

        Builder& set_tags(const Dictionary<string>::Builder& value) noexcept
        {
            b_tags = value;
            return *this;
        }

        Builder& set_what(Nat::Builder value) noexcept
        {
            b_what = value;
            return *this;
        }

        Builder& set_name(const string::Builder& value) noexcept
        requires(IS_SET(m_QUERY_FIELDS_MASK, 4))
        {
            b_name::value = value;
            return *this;
        }

        Builder& set_color(const string::Builder& value) noexcept
        requires(IS_SET(m_QUERY_FIELDS_MASK, 5))
        {
            b_color::value = value;
            return *this;
        }

        Builder& set_total(Int::Builder value) noexcept
        requires(IS_SET(m_QUERY_FIELDS_MASK, 6))
        {
            b_total::value = value;
            return *this;
        }

        Builder& set_max_hosts(const Array<string>::Builder& value) noexcept
        requires(IS_SET(m_QUERY_FIELDS_MASK, 7))
        {
            b_max_hosts::value = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            MAGIC.store(stream);
            m_fields_mask.store(stream);
            m_time_shift.store(stream);
            m_tags.store(stream);
            if (IS_SET(m_fields_mask, 1)) m_what.store(stream);
            if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 4)) b_name::value.store(stream);
            if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 5)) b_color::value.store(stream);
            if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 6)) b_total::value.store(stream);
            if constexpr (IS_SET(m_QUERY_FIELDS_MASK, 7)) b_max_hosts::value.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        Long::Builder b_time_shift;
        Dictionary<string>::Builder b_tags;
        Nat::Builder b_what;
    };

private:
    Nat m_fields_mask;
    Long m_time_shift;
    Dictionary<string> m_tags;
    Nat m_what;
};

}    // namespace baseline::statshouse
