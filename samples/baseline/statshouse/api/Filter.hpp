#pragma once

#include "prelude/baseline/Includes.hpp"

#include "samples/baseline/statshouse/api/TagValue.hpp"



namespace baseline::statshouse {

template <bool BOXED>
class Filter_BASE {
public:
    static constexpr Magic MAGIC = 70305850;
    static constexpr bool STATIC = false;

    Filter_BASE() noexcept = default;

    Filter_BASE(const Filter_BASE&) noexcept = default;
    Filter_BASE(Filter_BASE&&) noexcept = default;
    Filter_BASE& operator=(const Filter_BASE&) noexcept = default;
    Filter_BASE& operator=(Filter_BASE&&) noexcept = default;

    ~Filter_BASE() noexcept = default;

    const Nat& get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const string& get_key() const noexcept
    {
        return m_key;
    }

    const array<tagValue>& get_values() const noexcept
    {
        return m_values;
    }

    static Filter_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Nat fields_mask = Nat::fetch(stream);
        string key = string::fetch(stream);
        array<tagValue> value = array<tagValue>::fetch(stream);
        Filter_BASE result(std::move(fields_mask),
                           std::move(key),
                           std::move(value));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_fields_mask.store(stream);
        m_key.store(stream);
        m_values.store(stream);
    }

    class Builder {
    public:
        using TYPE = Filter_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const Filter_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_key == rhs.get_key()
                   && lhs.b_values == rhs.get_values();
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(Nat::Builder::random(engine))
                    .set_key(string::Builder::random<SIZE_1>(engine))
                    .set_values(array<tagValue>::Builder::random<SIZE_2, SIZE_3>(engine));
        }

        Builder& set_fields_mask(const Nat::Builder& value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_key(const string::Builder& value) noexcept
        {
            b_key = value;
            return *this;
        }

        Builder& set_values(const array<tagValue>::Builder& value) noexcept
        {
            b_values = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_fields_mask.store(stream);
            b_key.store(stream);
            b_values.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        string::Builder b_key;
        array<tagValue>::Builder b_values;
    };

private:
    Filter_BASE(Nat&& fields_mask,
                string&& key,
                array<tagValue>&& values) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_key(std::move(key))
        , m_values(std::move(values))
    {}

    Nat m_fields_mask;
    string m_key;
    array<tagValue> m_values;
};

using filter = Filter_BASE<false>;
using Filter = Filter_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const Filter_BASE<LHS_BOXED>& lhs, const Filter_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_key() == rhs.get_key()
           && lhs.get_values() == rhs.get_values();
}

template <bool BOXED>
size_t consume(const Filter_BASE<BOXED>& value) noexcept
{
    size_t result = 0;
    result += consume(value.get_fields_mask());
    result += consume(value.get_key());
    result += consume(value.get_values());
    return result;
}

}    // namespace baseline::statshouse
