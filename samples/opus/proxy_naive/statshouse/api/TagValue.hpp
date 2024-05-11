#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/proxy_naive/statshouse/api/Flag.hpp"



namespace opus::proxy_naive::statshouse {

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

    bool verify() const noexcept
    {
        if (!get_fields_mask().verify())
            return false;

        if (!get_in().verify())
            return false;

        if (!get_flag().verify())
            return false;

        return true;
    }

    Nat get_fields_mask() const noexcept
    {
        return Nat {m_proxy_1, PROXY_1_FIELDS_MASK_OFFSET};
    }

    Int get_in() const noexcept
    {
        return Int {m_proxy_1, PROXY_1_IN_OFFSET};
    }

    const string& get_value() const noexcept
    {
        return m_value;
    }

    Flag get_flag() const noexcept
    {
        return Flag {m_proxy_2, PROXY_2_FLAG_OFFSET};
    }

    static TagValue_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Proxy proxy_1 = Proxy::fetch(stream, PROXY_1_SIZEOF);
        string value = string::fetch(stream);
        Proxy proxy_2 = Proxy::fetch(stream, PROXY_2_SIZEOF);
        TagValue_BASE result(std::move(proxy_1),
                             std::move(value),
                             std::move(proxy_2));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_proxy_1.store(stream, PROXY_1_SIZEOF);
        m_value.store(stream);
        m_proxy_2.store(stream, PROXY_2_SIZEOF);
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
    static constexpr offset_t PROXY_1_SIZEOF = Nat::SIZEOF
                                               + Int::SIZEOF;
    static constexpr offset_t PROXY_1_FIELDS_MASK_OFFSET = 0;
    static constexpr offset_t PROXY_1_IN_OFFSET = PROXY_1_FIELDS_MASK_OFFSET + Nat::SIZEOF;

    static constexpr offset_t PROXY_2_SIZEOF = Flag::SIZEOF;
    static constexpr offset_t PROXY_2_FLAG_OFFSET = 0;

    TagValue_BASE(Proxy&& proxy_1,
                  string&& value,
                  Proxy&& proxy_2) noexcept
        : m_proxy_1(std::move(proxy_1))
        , m_value(std::move(value))
        , m_proxy_2(std::move(proxy_2))
    {}

    Proxy m_proxy_1;
    string m_value;
    Proxy m_proxy_2;
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

}    // namespace opus::proxy_naive::statshouse
