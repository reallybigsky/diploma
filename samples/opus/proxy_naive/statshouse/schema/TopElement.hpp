#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/proxy_naive/statshouse/schema/MultiValue.hpp"



namespace opus::proxy_naive::statshouse {

template <bool BOXED>
class TopElement_BASE {
public:
    static constexpr Magic MAGIC = 726786291;
    static constexpr bool STATIC = false;

    TopElement_BASE() noexcept = default;

    TopElement_BASE(const TopElement_BASE&) noexcept = default;
    TopElement_BASE(TopElement_BASE&&) noexcept = default;
    TopElement_BASE& operator=(const TopElement_BASE&) noexcept = default;
    TopElement_BASE& operator=(TopElement_BASE&&) noexcept = default;

    ~TopElement_BASE() noexcept = default;

    bool verify() const noexcept
    {
        if (!get_fields_mask().verify())
            return false;

        return true;
    }

    const string& get_key() const noexcept
    {
        return m_key;
    }

    Nat get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const multiValue& get_value() const noexcept
    {
        return m_value;
    }

    static TopElement_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        string key = string::fetch(stream);
        Nat fields_mask = Nat::fetch(stream);
        multiValue value = multiValue::fetch(stream, fields_mask);
        TopElement_BASE result(std::move(key),
                               std::move(fields_mask),
                               std::move(value));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_key.store(stream);
        m_fields_mask.store(stream);
        m_value.store(stream, get_fields_mask());
    }

    class Builder {
    public:
        using TYPE = TopElement_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const TopElement_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_key == rhs.get_key()
                   && lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_value == rhs.get_value();
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_key(string::Builder::random<SIZE_1>(engine))
                    .set_fields_mask(utils::random_mask<0, 511>(engine))
                    .set_value(multiValue::Builder::random<SIZE_2, SIZE_3>(engine));
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

        Builder& set_value(const multiValue::Builder& value) noexcept
        {
            b_value = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_key.store(stream);
            b_fields_mask.store(stream);
            b_value.store(stream, b_fields_mask);
        }

    private:
        string::Builder b_key;
        Nat::Builder b_fields_mask;
        multiValue::Builder b_value;
    };

private:
    TopElement_BASE(string&& key,
                    Nat&& fields_mask,
                    multiValue&& value) noexcept
        : m_key(std::move(key))
        , m_fields_mask(std::move(fields_mask))
        , m_value(std::move(value))
    {}

    string m_key;
    Nat m_fields_mask;
    multiValue m_value;
};

using topElement = TopElement_BASE<false>;
using TopElement = TopElement_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const TopElement_BASE<LHS_BOXED>& lhs, const TopElement_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_key() == rhs.get_key()
           && lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_value() == rhs.get_value();
}

}    // namespace opus::proxy_naive::statshouse
