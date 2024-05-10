#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/proxy/simple/Point.hpp"



namespace opus::proxy::simple {

template <bool BOXED, Nat...>
class Rectangle_BASE;

template <bool BOXED>
class Rectangle_BASE<BOXED> {
public:
    static constexpr Magic MAGIC = 2909707313;
    static constexpr bool STATIC = false;

    Rectangle_BASE() noexcept = default;

    Rectangle_BASE(const Rectangle_BASE&) noexcept = default;
    Rectangle_BASE(Rectangle_BASE&&) noexcept = default;
    Rectangle_BASE& operator=(const Rectangle_BASE&) noexcept = default;
    Rectangle_BASE& operator=(Rectangle_BASE&&) noexcept = default;

    ~Rectangle_BASE() noexcept = default;

    bool verify() const noexcept
    {
        return true;
    }

    const point& get_a() const noexcept
    {
        return m_a;
    }

    const point& get_b() const noexcept
    {
        return m_b;
    }

    static Rectangle_BASE fetch(InputStream& stream, Nat mask)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        point a = point::fetch(stream, mask);
        point b = point::fetch(stream, mask);
        Rectangle_BASE result(std::move(a),
                              std::move(b));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream, Nat mask) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_a.store(stream, mask);
        m_b.store(stream, mask);
    }

    class Builder {
    public:
        using TYPE = Rectangle_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const Rectangle_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_a == rhs.get_a()
                   && lhs.b_b == rhs.get_b();
        }

        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_a(point::Builder::random(engine))
                    .set_b(point::Builder::random(engine));
        }

        Builder& set_a(const point::Builder& value) noexcept
        {
            b_a = value;
            return *this;
        }

        Builder& set_b(const point::Builder& value) noexcept
        {
            b_b = value;
            return *this;
        }

        void store(OutputStream& stream, Nat mask) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_a.store(stream, mask);
            b_b.store(stream, mask);
        }

    private:
        point::Builder b_a;
        point::Builder b_b;
    };

private:
    Rectangle_BASE(point&& a,
                   point&& b) noexcept
        : m_a(std::move(a))
        , m_b(std::move(b))
    {}

    point m_a;
    point m_b;
};

template <bool BOXED, Nat MASK>
class Rectangle_BASE<BOXED, MASK> {
public:
    static constexpr Magic MAGIC = utils::commutative(2909707313, MASK);
    static constexpr offset_t SIZEOF = (BOXED ? Magic::SIZEOF : 0)
                                       + point_s<MASK>::SIZEOF
                                       + point_s<MASK>::SIZEOF;
    static constexpr bool STATIC = true;

    Rectangle_BASE() noexcept = default;

    Rectangle_BASE(const Rectangle_BASE&) noexcept = default;
    Rectangle_BASE(Rectangle_BASE&&) noexcept = default;
    Rectangle_BASE& operator=(const Rectangle_BASE&) noexcept = default;
    Rectangle_BASE& operator=(Rectangle_BASE&&) noexcept = default;

    ~Rectangle_BASE() noexcept = default;

    Rectangle_BASE(const Proxy& proxy, offset_t offset) noexcept
        : m_proxy(proxy, offset)
    {}

    bool verify() const noexcept
    {
        if constexpr (BOXED)
            if (*(Magic*)(m_proxy.data()) != MAGIC)
                return false;

        if (!get_a().verify())
            return false;

        if (!get_b().verify())
            return false;

        return true;
    }

    point_s<MASK> get_a() const noexcept
    {
        return point_s<MASK> {m_proxy, A_OFFSET};
    }

    point_s<MASK> get_b() const noexcept
    {
        return point_s<MASK> {m_proxy, B_OFFSET};
    }

    static Rectangle_BASE fetch(InputStream& stream)
    {
        Proxy proxy = Proxy::fetch(stream, SIZEOF);
        Rectangle_BASE result(std::move(proxy));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_proxy.store(stream, SIZEOF);
    }

    class Builder {
    public:
        using TYPE = Rectangle_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const Rectangle_BASE<RHS_BOXED, MASK>& rhs) noexcept
        {
            return lhs.b_a == rhs.get_a()
                   && lhs.b_b == rhs.get_b();
        }

        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_a(point_s<MASK>::Builder::random(engine))
                    .set_b(point_s<MASK>::Builder::random(engine));
        }

        Builder& set_a(const point_s<MASK>::Builder& value) noexcept
        {
            b_a = value;
            return *this;
        }

        Builder& set_b(const point_s<MASK>::Builder& value) noexcept
        {
            b_b = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_a.store(stream);
            b_b.store(stream);
        }

    private:
        point_s<MASK>::Builder b_a;
        point_s<MASK>::Builder b_b;
    };

private:
    static constexpr offset_t A_OFFSET = BOXED ? Magic::SIZEOF : 0;
    static constexpr offset_t B_OFFSET = A_OFFSET + point_s<MASK>::SIZEOF;

    Rectangle_BASE(Proxy&& proxy) noexcept
        : m_proxy(std::move(proxy))
    {}

    Proxy m_proxy;
};

using rectangle = Rectangle_BASE<false>;
using Rectangle = Rectangle_BASE<true>;

template <Nat MASK>
using rectangle_s = Rectangle_BASE<false, MASK>;
template <Nat MASK>
using Rectangle_s = Rectangle_BASE<true, MASK>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const Rectangle_BASE<LHS_BOXED>& lhs, const Rectangle_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_a() == rhs.get_a()
           && lhs.get_b() == rhs.get_b();
}

template <bool LHS_BOXED, bool RHS_BOXED, Nat MASK>
bool operator==(const Rectangle_BASE<LHS_BOXED, MASK>& lhs, const Rectangle_BASE<RHS_BOXED, MASK>& rhs) noexcept
{
    if (lhs.get_a() != rhs.get_a())
        return false;

    if (lhs.get_b() != rhs.get_b())
        return false;

    return true;
}

}    // namespace opus::proxy::simple
