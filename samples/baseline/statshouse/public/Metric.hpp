#pragma once

#include "baseline/Includes.hpp"
#include "baseline/statshouse/Dictionary.hpp"



namespace baseline::statshouse {

class Metric {
public:
    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
    static constexpr Magic MAGIC = 3332106996;
    static constexpr bool CTS = false;

    Metric() noexcept = default;

    Metric(const Metric&) noexcept = default;
    Metric(Metric&&) noexcept = default;
    Metric& operator=(const Metric&) noexcept = default;
    Metric& operator=(Metric&&) noexcept = default;

    ~Metric() noexcept = default;

    Metric(Nat fields_mask,
           const string& name,
           const Dictionary<string>& tags,
           Double counter,
           Nat ts,
           const array<Double>& value,
           const array<Long>& unique) noexcept
        : m_fields_mask(fields_mask)
        , m_name(name)
        , m_tags(tags)
        , m_counter(counter)
        , m_ts(ts)
        , m_value(value)
        , m_unique(unique)
    {}

    Nat get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const string& get_name() const noexcept
    {
        return m_name;
    }

    const Dictionary<string>& get_tags() const noexcept
    {
        return m_tags;
    }

    Double get_counter() const noexcept
    {
        return m_counter;
    }

    Nat get_ts() const noexcept
    {
        return m_ts;
    }

    const array<Double>& get_value() const noexcept
    {
        return m_value;
    }

    const array<Long>& get_unique() const noexcept
    {
        return m_unique;
    }

    static Metric fetch(InputStream& stream)
    {
        Magic magic = Magic::fetch(stream);
        if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);

        Nat fields_mask = Nat::fetch(stream);
        string name = string::fetch(stream);
        Dictionary<string> tags = Dictionary<string>::fetch(stream);
        Double counter;
        if (IS_SET(fields_mask, 0)) counter = Double::fetch(stream);
        Nat ts;
        if (IS_SET(fields_mask, 4)) ts = Nat::fetch(stream);
        array<Double> value;
        if (IS_SET(fields_mask, 1)) value = array<Double>::fetch(stream);
        array<Long> unique;
        if (IS_SET(fields_mask, 2)) unique = array<Long>::fetch(stream);
        return {fields_mask,
                name,
                tags,
                counter,
                ts,
                value,
                unique};
    }

    void store(OutputStream& stream) const
    {
        MAGIC.store(stream);
        m_fields_mask.store(stream);
        m_name.store(stream);
        m_tags.store(stream);
        if (IS_SET(m_fields_mask, 0)) m_counter.store(stream);
        if (IS_SET(m_fields_mask, 4)) m_ts.store(stream);
        if (IS_SET(m_fields_mask, 1)) m_value.store(stream);
        if (IS_SET(m_fields_mask, 2)) m_unique.store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const Metric& rhs)
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_name == rhs.get_name()
                   && lhs.b_tags == rhs.get_tags()
                   && (!IS_SET(lhs.b_fields_mask, 0) || lhs.b_counter == rhs.get_counter())
                   && (!IS_SET(lhs.b_fields_mask, 4) || lhs.b_ts == rhs.get_ts())
                   && (!IS_SET(lhs.b_fields_mask, 1) || lhs.b_value == rhs.get_value())
                   && (!IS_SET(lhs.b_fields_mask, 2) || lhs.b_unique == rhs.get_unique());
        }

        Builder& set_fields_mask(Nat::Builder value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_name(const string::Builder& value) noexcept
        {
            b_name = value;
            return *this;
        }

        Builder& set_tags(const Dictionary<string>::Builder& value) noexcept
        {
            b_tags = value;
            return *this;
        }

        Builder& set_counter(Double::Builder value) noexcept
        {
            b_counter = value;
            return *this;
        }

        Builder& set_ts(Nat::Builder value) noexcept
        {
            b_ts = value;
            return *this;
        }

        Builder& set_value(const array<Double>::Builder& value) noexcept
        {
            b_value = value;
            return *this;
        }

        Builder& set_unique(const array<Long>::Builder& value) noexcept
        {
            b_unique = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            MAGIC.store(stream);
            b_fields_mask.store(stream);
            b_name.store(stream);
            b_tags.store(stream);
            if (IS_SET(b_fields_mask, 0)) b_counter.store(stream);
            if (IS_SET(b_fields_mask, 4)) b_ts.store(stream);
            if (IS_SET(b_fields_mask, 1)) b_value.store(stream);
            if (IS_SET(b_fields_mask, 2)) b_unique.store(stream);
        }

    private:
        Nat b_fields_mask;
        string::Builder b_name;
        Dictionary<string>::Builder b_tags;
        Double::Builder b_counter;
        Nat::Builder b_ts;
        array<Double>::Builder b_value;
        array<Long>::Builder b_unique;
    };

private:
    Nat m_fields_mask;
    string m_name;
    Dictionary<string> m_tags;
    Double m_counter;
    Nat m_ts;
    array<Double> m_value;
    array<Long> m_unique;
};

}    // namespace baseline::statshouse
