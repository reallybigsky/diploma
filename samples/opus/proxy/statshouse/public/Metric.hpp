#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/proxy/statshouse/Dictionary.hpp"



namespace opus::proxy::statshouse {

template <bool BOXED>
class Metric_BASE {
public:
    static constexpr Magic MAGIC = 3332106996;
    static constexpr bool STATIC = false;

    Metric_BASE() noexcept = default;

    Metric_BASE(const Metric_BASE&) noexcept = default;
    Metric_BASE(Metric_BASE&&) noexcept = default;
    Metric_BASE& operator=(const Metric_BASE&) noexcept = default;
    Metric_BASE& operator=(Metric_BASE&&) noexcept = default;

    ~Metric_BASE() noexcept = default;

    bool verify() const noexcept
    {
        if (!get_fields_mask().verify())
            return false;

        if (const auto& counter = get_counter(); counter && !counter->verify())
            return false;

        if (const auto& ts = get_ts(); ts && !ts->verify())
            return false;

        return true;
    }

    Nat get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const string& get_name() const noexcept
    {
        return m_name;
    }

    const dictionary<string>& get_tags() const noexcept
    {
        return m_tags;
    }

    std::optional<Double> get_counter() const noexcept
    {
        return m_counter;
    }

    std::optional<Nat> get_ts() const noexcept
    {
        return m_ts;
    }

    const std::optional<array<Double>>& get_value() const noexcept
    {
        return m_value;
    }

    const std::optional<array<Long>>& get_unique() const noexcept
    {
        return m_unique;
    }

    static Metric_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Nat fields_mask = Nat::fetch(stream);
        string name = string::fetch(stream);
        dictionary<string> tags = dictionary<string>::fetch(stream);
        std::optional<Double> counter;
        if (IS_SET(fields_mask, 0)) counter = Double::fetch(stream);
        std::optional<Nat> ts;
        if (IS_SET(fields_mask, 4)) ts = Nat::fetch(stream);
        std::optional<array<Double>> value;
        if (IS_SET(fields_mask, 1)) value = array<Double>::fetch(stream);
        std::optional<array<Long>> unique;
        if (IS_SET(fields_mask, 2)) unique = array<Long>::fetch(stream);
        Metric_BASE result {std::move(fields_mask),
                            std::move(name),
                            std::move(tags),
                            std::move(counter),
                            std::move(ts),
                            std::move(value),
                            std::move(unique)};
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_fields_mask.store(stream);
        m_name.store(stream);
        m_tags.store(stream);
        if (IS_SET(get_fields_mask(), 0)) m_counter->store(stream);
        if (IS_SET(get_fields_mask(), 4)) m_ts->store(stream);
        if (IS_SET(get_fields_mask(), 1)) m_value->store(stream);
        if (IS_SET(get_fields_mask(), 2)) m_unique->store(stream);
    }

    class Builder {
    public:
        using TYPE = Metric_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const Metric_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_name == rhs.get_name()
                   && lhs.b_tags == rhs.get_tags()
                   && (!rhs.get_counter() || lhs.b_counter == rhs.get_counter())
                   && (!rhs.get_ts() || lhs.b_ts == rhs.get_ts())
                   && (!rhs.get_value() || lhs.b_value == rhs.get_value())
                   && (!rhs.get_unique() || lhs.b_unique == rhs.get_unique());
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3, size_t SIZE_4, size_t SIZE_5, size_t SIZE_6>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(utils::random_mask<0, 31>(engine))
                    .set_name(string::Builder::random<SIZE_1>(engine))
                    .set_tags(dictionary<string>::Builder::random<SIZE_2, SIZE_3, SIZE_4>(engine))
                    .set_counter(Double::Builder::random(engine))
                    .set_ts(Nat::Builder::random(engine))
                    .set_value(array<Double>::Builder::random<SIZE_5>(engine))
                    .set_unique(array<Long>::Builder::random<SIZE_6>(engine));
        }

        Builder& set_fields_mask(const Nat::Builder& value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_name(const string::Builder& value) noexcept
        {
            b_name = value;
            return *this;
        }

        Builder& set_tags(const dictionary<string>::Builder& value) noexcept
        {
            b_tags = value;
            return *this;
        }

        Builder& set_counter(const Double::Builder& value) noexcept
        {
            b_counter = value;
            return *this;
        }

        Builder& set_ts(const Nat::Builder& value) noexcept
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
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_fields_mask.store(stream);
            b_name.store(stream);
            b_tags.store(stream);
            if (IS_SET(b_fields_mask, 0)) b_counter.store(stream);
            if (IS_SET(b_fields_mask, 4)) b_ts.store(stream);
            if (IS_SET(b_fields_mask, 1)) b_value.store(stream);
            if (IS_SET(b_fields_mask, 2)) b_unique.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        string::Builder b_name;
        dictionary<string>::Builder b_tags;
        Double::Builder b_counter;
        Nat::Builder b_ts;
        array<Double>::Builder b_value;
        array<Long>::Builder b_unique;
    };

private:
    Metric_BASE(Nat&& fields_mask,
                string&& name,
                dictionary<string>&& tags,
                std::optional<Double>&& counter,
                std::optional<Nat>&& ts,
                std::optional<array<Double>>&& value,
                std::optional<array<Long>>&& unique) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_name(std::move(name))
        , m_tags(std::move(tags))
        , m_counter(std::move(counter))
        , m_ts(std::move(ts))
        , m_value(std::move(value))
        , m_unique(std::move(unique))
    {}

    Nat m_fields_mask;
    string m_name;
    dictionary<string> m_tags;
    std::optional<Double> m_counter;
    std::optional<Nat> m_ts;
    std::optional<array<Double>> m_value;
    std::optional<array<Long>> m_unique;
};

using metric = Metric_BASE<false>;
using Metric = Metric_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const Metric_BASE<LHS_BOXED>& lhs, const Metric_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_name() == rhs.get_name()
           && lhs.get_tags() == rhs.get_tags()
           && lhs.get_counter() == rhs.get_counter()
           && lhs.get_ts() == rhs.get_ts()
           && lhs.get_value() == rhs.get_value()
           && lhs.get_unique() == rhs.get_unique();
}

}    // namespace opus::proxy::statshouse
