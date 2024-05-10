#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::proxy_naive::simple {

template <bool BOXED, Nat...>
class Point_BASE;

template <bool BOXED>
class Point_BASE<BOXED> {
public:
    static constexpr Magic MAGIC = 2850977279;
    static constexpr bool STATIC = false;

    Point_BASE() noexcept = default;

    Point_BASE(const Point_BASE&) noexcept = default;
    Point_BASE(Point_BASE&&) noexcept = default;
    Point_BASE& operator=(const Point_BASE&) noexcept = default;
    Point_BASE& operator=(Point_BASE&&) noexcept = default;

    ~Point_BASE() noexcept = default;

    bool verify() const noexcept
    {
        if (const auto& x = get_x(); x && !x->verify())
            return false;

        if (const auto& y = get_y(); y && !y->verify())
            return false;

        if (const auto& z = get_z(); z && !z->verify())
            return false;

        return true;
    }

    std::optional<Long> get_x() const noexcept
    {
        return PROXY_1_LONG1_OFFSET == NO_VALUE ? std::optional<Long>() : Long {m_proxy_1, PROXY_1_LONG1_OFFSET};
    }

    std::optional<Long> get_y() const noexcept
    {
        return PROXY_1_LONG2_OFFSET == NO_VALUE ? std::optional<Long>() : Long {m_proxy_1, PROXY_1_LONG2_OFFSET};
    }

    std::optional<Long> get_z() const noexcept
    {
        return PROXY_1_LONG3_OFFSET == NO_VALUE ? std::optional<Long>() : Long {m_proxy_1, PROXY_1_LONG3_OFFSET};
    }

    static Point_BASE fetch(InputStream& stream, Nat mask)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        offset_t PROXY_1_SIZEOF = (IS_SET(mask, 0) ? Long::SIZEOF : 0)
                                  + (IS_SET(mask, 1) ? Long::SIZEOF : 0)
                                  + (IS_SET(mask, 2) ? Long::SIZEOF : 0);
        offset_t proxy_1_pos = 0;
        offset_t PROXY_1_LONG1_OFFSET = NO_VALUE;
        offset_t PROXY_1_LONG2_OFFSET = NO_VALUE;
        offset_t PROXY_1_LONG3_OFFSET = NO_VALUE;
        if (IS_SET(mask, 0)) {
            PROXY_1_LONG1_OFFSET = proxy_1_pos;
            proxy_1_pos += Long::SIZEOF;
        }
        if (IS_SET(mask, 1)) {
            PROXY_1_LONG2_OFFSET = proxy_1_pos;
            proxy_1_pos += Long::SIZEOF;
        }
        if (IS_SET(mask, 2)) {
            PROXY_1_LONG3_OFFSET = proxy_1_pos;
            proxy_1_pos += Long::SIZEOF;
        }
        Proxy proxy_1 = Proxy::fetch(stream, PROXY_1_SIZEOF);
        Point_BASE result(std::move(PROXY_1_SIZEOF),
                          std::move(PROXY_1_LONG1_OFFSET),
                          std::move(PROXY_1_LONG2_OFFSET),
                          std::move(PROXY_1_LONG3_OFFSET),
                          std::move(proxy_1));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream, Nat) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_proxy_1.store(stream, PROXY_1_SIZEOF);
    }

    class Builder {
    public:
        using TYPE = Point_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const Point_BASE<RHS_BOXED>& rhs) noexcept
        {
            return (!rhs.get_x() || lhs.b_x == rhs.get_x())
                   && (!rhs.get_y() || lhs.b_y == rhs.get_y())
                   && (!rhs.get_z() || lhs.b_z == rhs.get_z());
        }

        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_x(Long::Builder::random(engine))
                    .set_y(Long::Builder::random(engine))
                    .set_z(Long::Builder::random(engine));
        }

        Builder& set_x(const Long::Builder& value) noexcept
        {
            b_x = value;
            return *this;
        }

        Builder& set_y(const Long::Builder& value) noexcept
        {
            b_y = value;
            return *this;
        }

        Builder& set_z(const Long::Builder& value) noexcept
        {
            b_z = value;
            return *this;
        }

        void store(OutputStream& stream, Nat mask) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            if (IS_SET(mask, 0)) b_x.store(stream);
            if (IS_SET(mask, 1)) b_y.store(stream);
            if (IS_SET(mask, 2)) b_z.store(stream);
        }

    private:
        Long::Builder b_x;
        Long::Builder b_y;
        Long::Builder b_z;
    };

private:
    Point_BASE(offset_t&& PROXY_1_SIZEOF,
               offset_t&& PROXY_1_LONG1_OFFSET,
               offset_t&& PROXY_1_LONG2_OFFSET,
               offset_t&& PROXY_1_LONG3_OFFSET,
               Proxy&& proxy_1) noexcept
        : PROXY_1_SIZEOF(std::move(PROXY_1_SIZEOF))
        , PROXY_1_LONG1_OFFSET(std::move(PROXY_1_LONG1_OFFSET))
        , PROXY_1_LONG2_OFFSET(std::move(PROXY_1_LONG2_OFFSET))
        , PROXY_1_LONG3_OFFSET(std::move(PROXY_1_LONG3_OFFSET))
        , m_proxy_1(std::move(proxy_1))
    {}

    offset_t PROXY_1_SIZEOF = 0;
    offset_t PROXY_1_LONG1_OFFSET = NO_VALUE;
    offset_t PROXY_1_LONG2_OFFSET = NO_VALUE;
    offset_t PROXY_1_LONG3_OFFSET = NO_VALUE;
    Proxy m_proxy_1;
};

template <bool BOXED, Nat MASK>
class Point_BASE<BOXED, MASK> {
public:
    static constexpr Magic MAGIC = utils::commutative(2850977279, MASK);
    static constexpr offset_t SIZEOF = (BOXED ? Magic::SIZEOF : 0)
                                       + (IS_SET(MASK, 0) ? Long::SIZEOF : 0)
                                       + (IS_SET(MASK, 1) ? Long::SIZEOF : 0)
                                       + (IS_SET(MASK, 2) ? Long::SIZEOF : 0);
    static constexpr bool STATIC = true;

    Point_BASE() noexcept = default;

    Point_BASE(const Point_BASE&) noexcept = default;
    Point_BASE(Point_BASE&&) noexcept = default;
    Point_BASE& operator=(const Point_BASE&) noexcept = default;
    Point_BASE& operator=(Point_BASE&&) noexcept = default;

    ~Point_BASE() noexcept = default;

    Point_BASE(const Proxy& proxy, offset_t offset) noexcept
        : m_proxy(proxy, offset)
    {}

    bool verify() const noexcept
    {
        if constexpr (BOXED)
            if (*(Magic*)(m_proxy.data()) != MAGIC)
                return false;

        if constexpr (IS_SET(MASK, 0))
            if (!get_x().verify())
                return false;

        if constexpr (IS_SET(MASK, 1))
            if (!get_y().verify())
                return false;

        if constexpr (IS_SET(MASK, 2))
            if (!get_z().verify())
                return false;

        return true;
    }

    Long get_x() const noexcept
    requires(IS_SET(MASK, 0))
    {
        return Long {m_proxy, X_OFFSET};
    }

    Long get_y() const noexcept
    requires(IS_SET(MASK, 1))
    {
        return Long {m_proxy, Y_OFFSET};
    }

    Long get_z() const noexcept
    requires(IS_SET(MASK, 2))
    {
        return Long {m_proxy, Z_OFFSET};
    }

    static Point_BASE fetch(InputStream& stream)
    {
        Proxy proxy = Proxy::fetch(stream, SIZEOF);
        Point_BASE result(std::move(proxy));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_proxy.store(stream, SIZEOF);
    }

    class Builder
        : private MAYBE<Long::Builder, IS_SET(MASK, 0), 0>
        , private MAYBE<Long::Builder, IS_SET(MASK, 1), 1>
        , private MAYBE<Long::Builder, IS_SET(MASK, 2), 2> {
    private:
        using b_x = MAYBE<Long::Builder, IS_SET(MASK, 0), 0>;
        using b_y = MAYBE<Long::Builder, IS_SET(MASK, 1), 1>;
        using b_z = MAYBE<Long::Builder, IS_SET(MASK, 2), 2>;

    public:
        using TYPE = Point_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const Point_BASE<RHS_BOXED, MASK>& rhs) noexcept
        {
            if constexpr (IS_SET(MASK, 0))
                if (lhs.b_x::value != rhs.get_x())
                    return false;

            if constexpr (IS_SET(MASK, 1))
                if (lhs.b_y::value != rhs.get_y())
                    return false;

            if constexpr (IS_SET(MASK, 2))
                if (lhs.b_z::value != rhs.get_z())
                    return false;

            return true;
        }

        static Builder random(std::default_random_engine& engine) noexcept
        {
            Builder result;
            if constexpr (IS_SET(MASK, 0)) result.set_x(Long::Builder::random(engine));
            if constexpr (IS_SET(MASK, 1)) result.set_y(Long::Builder::random(engine));
            if constexpr (IS_SET(MASK, 2)) result.set_z(Long::Builder::random(engine));
            return result;
        }

        Builder& set_x(const Long::Builder& value) noexcept
        requires(IS_SET(MASK, 0))
        {
            b_x::value = value;
            return *this;
        }

        Builder& set_y(const Long::Builder& value) noexcept
        requires(IS_SET(MASK, 1))
        {
            b_y::value = value;
            return *this;
        }

        Builder& set_z(const Long::Builder& value) noexcept
        requires(IS_SET(MASK, 2))
        {
            b_z::value = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            if constexpr (IS_SET(MASK, 0)) b_x::value.store(stream);
            if constexpr (IS_SET(MASK, 1)) b_y::value.store(stream);
            if constexpr (IS_SET(MASK, 2)) b_z::value.store(stream);
        }

    private:
    };

private:
    static constexpr offset_t X_OFFSET = BOXED ? Magic::SIZEOF : 0;
    static constexpr offset_t Y_OFFSET = X_OFFSET + (IS_SET(MASK, 0) ? Long::SIZEOF : 0);
    static constexpr offset_t Z_OFFSET = Y_OFFSET + (IS_SET(MASK, 1) ? Long::SIZEOF : 0);

    Point_BASE(Proxy&& proxy) noexcept
        : m_proxy(std::move(proxy))
    {}

    Proxy m_proxy;
};

using point = Point_BASE<false>;
using Point = Point_BASE<true>;

template <Nat MASK>
using point_s = Point_BASE<false, MASK>;
template <Nat MASK>
using Point_s = Point_BASE<true, MASK>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const Point_BASE<LHS_BOXED>& lhs, const Point_BASE<RHS_BOXED>& rhs)
{
    return lhs.get_x() == rhs.get_x()
           && lhs.get_y() == rhs.get_y()
           && lhs.get_z() == rhs.get_z();
}

template <bool LHS_BOXED, bool RHS_BOXED, Nat MASK>
bool operator==(const Point_BASE<LHS_BOXED, MASK>& lhs, const Point_BASE<RHS_BOXED, MASK>& rhs)
{
    if constexpr (IS_SET(MASK, 0))
        if (lhs.get_x() != rhs.get_x())
            return false;

    if constexpr (IS_SET(MASK, 1))
        if (lhs.get_y() != rhs.get_y())
            return false;

    if constexpr (IS_SET(MASK, 2))
        if (lhs.get_z() != rhs.get_z())
            return false;

    return true;
}

}    // namespace opus::proxy_naive::simple
