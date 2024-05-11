#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/inline/statshouse/api/Flag.hpp"



namespace opus::inl::statshouse {

template <bool BOXED>
class TagValue_BASE {
public:
    static constexpr Magic MAGIC = 250552490;
    static constexpr bool STATIC = false;

    TagValue_BASE() noexcept = default;

    TagValue_BASE(const TagValue_BASE&) noexcept = default;
    TagValue_BASE(TagValue_BASE&&) noexcept = default;
    TagValue_BASE& operator=(const TagValue_BASE&) noexcept = default;
    TagValue_BASE& operator=(TagValue_BASE&&) noexcept = default;

    ~TagValue_BASE() noexcept = default;

    const Nat& get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const Int& get_in() const noexcept
    {
        return m_in;
    }

    const string& get_value() const noexcept
    {
        return m_value;
    }

    const Flag& get_flag() const noexcept
    {
        return m_flag;
    }

    static TagValue_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Nat fields_mask = Nat::fetch(stream);
        Int in = Int::fetch(stream);
        string value = string::fetch(stream);
        Flag flag = Flag::fetch(stream);
        TagValue_BASE result(std::move(fields_mask),
                             std::move(in),
                             std::move(value),
                             std::move(flag));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_fields_mask.store(stream);
        m_in.store(stream);
        m_value.store(stream);
        m_flag.store(stream);
    }

    class Builder {
    public:
        using TYPE = TagValue_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const TagValue_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_in == rhs.get_in()
                   && lhs.b_value == rhs.get_value()
                   && lhs.b_flag == rhs.get_flag();
        }

        template <size_t SIZE>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(Nat::Builder::random(engine))
                    .set_in(Int::Builder::random(engine))
                    .set_value(string::Builder::random<SIZE>(engine))
                    .set_flag(Flag::Builder::random(engine));
        }

        Builder& set_fields_mask(const Nat::Builder& value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_in(const Int::Builder& value) noexcept
        {
            b_in = value;
            return *this;
        }

        Builder& set_value(const string::Builder& value) noexcept
        {
            b_value = value;
            return *this;
        }

        Builder& set_flag(const Flag::Builder& value) noexcept
        {
            b_flag = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_fields_mask.store(stream);
            b_in.store(stream);
            b_value.store(stream);
            b_flag.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        Int::Builder b_in;
        string::Builder b_value;
        Flag::Builder b_flag;
    };

private:
    TagValue_BASE(Nat&& fields_mask,
                  Int&& in,
                  string&& value,
                  Flag&& flag) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_in(std::move(in))
        , m_value(std::move(value))
        , m_flag(std::move(flag))
    {}

    Nat m_fields_mask;
    Int m_in;
    string m_value;
    Flag m_flag;
};

using tagValue = TagValue_BASE<false>;
using TagValue = TagValue_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const TagValue_BASE<LHS_BOXED>& lhs, const TagValue_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_in() == rhs.get_in()
           && lhs.get_value() == rhs.get_value()
           && lhs.get_flag() == rhs.get_flag();
}

template <bool BOXED>
size_t consume(const TagValue_BASE<BOXED>& value) noexcept
{
    size_t result = 0;
    result += consume(value.get_fields_mask());
    result += consume(value.get_in());
    result += consume(value.get_value());
    result += consume(value.get_flag());
    return result;
}

}    // namespace opus::inl::statshouse
