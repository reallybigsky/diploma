#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/proxy_naive/simple/Rectangle.hpp"



namespace opus::proxy_naive::simple {

template <bool BOXED>
class Picture_BASE {
public:
    static constexpr Magic MAGIC = COMPILE_TIME_CRC32_STR("Picture");
    static constexpr bool STATIC = false;

    Picture_BASE() noexcept = default;

    Picture_BASE(const Picture_BASE&) noexcept = default;
    Picture_BASE(Picture_BASE&&) noexcept = default;
    Picture_BASE& operator=(const Picture_BASE&) noexcept = default;
    Picture_BASE& operator=(Picture_BASE&&) noexcept = default;

    ~Picture_BASE() noexcept = default;

    bool verify() const noexcept
    {
        if (!get_fields_mask().verify())
            return false;

        return true;
    }

    Nat get_fields_mask() const noexcept
    {
        return Nat {m_proxy_1, PROXY_1_FIELDS_MASK_OFFSET};
    }

    const Rectangle& get_r() const noexcept
    {
        return m_r;
    }

    static Picture_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Proxy proxy_1 = Proxy::fetch(stream, PROXY_1_SIZEOF);
        Nat fields_mask(proxy_1, PROXY_1_FIELDS_MASK_OFFSET);
        Rectangle r = Rectangle::fetch(stream, fields_mask);
        Picture_BASE result(std::move(proxy_1),
                            std::move(r));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_proxy_1.store(stream, PROXY_1_SIZEOF);
        m_r.store(stream, get_fields_mask());
    }

    class Builder {
    public:
        using TYPE = Picture_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const Picture_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_r == rhs.get_r();
        }

        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(random_mask<7>(engine))
                    .set_r(Rectangle::Builder::random(engine));
        }

        Builder& set_fields_mask(const Nat::Builder& value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_r(const Rectangle::Builder& value) noexcept
        {
            b_r = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_fields_mask.store(stream);
            b_r.store(stream, b_fields_mask);
        }

    private:
        Nat::Builder b_fields_mask;
        Rectangle::Builder b_r;
    };

private:
    static constexpr offset_t PROXY_1_SIZEOF = Nat::SIZEOF;
    static constexpr offset_t PROXY_1_FIELDS_MASK_OFFSET = 0;

    Picture_BASE(Proxy&& proxy_1,
                 Rectangle&& r) noexcept
        : m_proxy_1(std::move(proxy_1))
        , m_r(std::move(r))
    {}

    Proxy m_proxy_1;
    Rectangle m_r;
};

using picture = Picture_BASE<false>;
using Picture = Picture_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const Picture_BASE<LHS_BOXED>& lhs, const Picture_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_r() == rhs.get_r();
}

}    // namespace opus::proxy_naive::simple
