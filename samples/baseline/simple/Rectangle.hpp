#pragma once

#include "prelude/baseline/Includes.hpp"
#include <cstddef>

#include "samples/baseline/simple/Point.hpp"



namespace baseline::simple {

template <bool BOXED, Nat...>
class Rectangle_BASE;

template <bool BOXED>
class Rectangle_BASE<BOXED> {
public:
    static constexpr Magic MAGIC = COMPILE_TIME_CRC32_STR("Rectangle");
    static constexpr bool STATIC = false;

    Rectangle_BASE() noexcept = default;

    Rectangle_BASE(const Rectangle_BASE&) noexcept = default;
    Rectangle_BASE(Rectangle_BASE&&) noexcept = default;
    Rectangle_BASE& operator=(const Rectangle_BASE&) noexcept = default;
    Rectangle_BASE& operator=(Rectangle_BASE&&) noexcept = default;

    ~Rectangle_BASE() noexcept = default;

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
class Rectangle_BASE<BOXED, MASK>
    : private MAYBE<Magic, BOXED, COMPILE_TIME_CRC32_STR("Rectangle")> {
private:
    using m_magic = MAYBE<Magic, BOXED, COMPILE_TIME_CRC32_STR("Rectangle")>;

public:
    static constexpr Magic MAGIC = COMPILE_TIME_CRC32_STR("Rectangle");
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

    const point_s<MASK>& get_a() const noexcept
    {
        return m_a;
    }

    const point_s<MASK>& get_b() const noexcept
    {
        return m_b;
    }

    static Rectangle_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        point_s<MASK> a = point_s<MASK>::fetch(stream);
        point_s<MASK> b = point_s<MASK>::fetch(stream);
        Rectangle_BASE result(std::move(a),
                              std::move(b));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_a.store(stream);
        m_b.store(stream);
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
    Rectangle_BASE(point_s<MASK>&& a,
                   point_s<MASK>&& b) noexcept
        : m_magic(MAGIC)
        , m_a(std::move(a))
        , m_b(std::move(b))
    {}

    point_s<MASK> m_a;
    point_s<MASK> m_b;
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

}    // namespace baseline::simple
