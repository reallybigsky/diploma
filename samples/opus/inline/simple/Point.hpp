#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::inl::simple {

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

    const std::optional<Long>& get_x() const noexcept
    {
        return m_x;
    }

    const std::optional<Long>& get_y() const noexcept
    {
        return m_y;
    }

    const std::optional<Long>& get_z() const noexcept
    {
        return m_z;
    }

    static Point_BASE fetch(InputStream& stream, Nat mask)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        std::optional<Long> x;
        if (IS_SET(mask, 0)) x = Long::fetch(stream);
        std::optional<Long> y;
        if (IS_SET(mask, 1)) y = Long::fetch(stream);
        std::optional<Long> z;
        if (IS_SET(mask, 2)) z = Long::fetch(stream);
        Point_BASE result(std::move(x),
                          std::move(y),
                          std::move(z));
        return result;
    }

    void store(OutputStream& stream, Nat mask) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        if (IS_SET(mask, 0)) m_x->store(stream);
        if (IS_SET(mask, 1)) m_y->store(stream);
        if (IS_SET(mask, 2)) m_z->store(stream);
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
    Point_BASE(std::optional<Long>&& x,
               std::optional<Long>&& y,
               std::optional<Long>&& z) noexcept
        : m_x(std::move(x))
        , m_y(std::move(y))
        , m_z(std::move(z))
    {}

    std::optional<Long> m_x;
    std::optional<Long> m_y;
    std::optional<Long> m_z;
};

template <bool BOXED, Nat MASK>
class Point_BASE<BOXED, MASK>
    : private MAYBE<Magic, BOXED, utils::commutative(2850977279, MASK)>
    , private MAYBE<Long, IS_SET(MASK, 0), utils::commutative(2850977279, 0)>
    , private MAYBE<Long, IS_SET(MASK, 1), utils::commutative(2850977279, 1)>
    , private MAYBE<Long, IS_SET(MASK, 2), utils::commutative(2850977279, 2)> {
private:
    using m_magic = MAYBE<Magic, BOXED, utils::commutative(2850977279, MASK)>;
    using m_x = MAYBE<Long, IS_SET(MASK, 0), utils::commutative(2850977279, 0)>;
    using m_y = MAYBE<Long, IS_SET(MASK, 1), utils::commutative(2850977279, 1)>;
    using m_z = MAYBE<Long, IS_SET(MASK, 2), utils::commutative(2850977279, 2)>;

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
        : m_magic(proxy, offset)
        , m_x(proxy, offset + X_OFFSET)
        , m_y(proxy, offset + Y_OFFSET)
        , m_z(proxy, offset + Z_OFFSET)
    {}

    bool verify() const noexcept
    {
        if constexpr (BOXED)
            if (m_magic::value != MAGIC)
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

    const Long& get_x() const noexcept
    requires(IS_SET(MASK, 0))
    {
        return m_x::value;
    }

    const Long& get_y() const noexcept
    requires(IS_SET(MASK, 1))
    {
        return m_y::value;
    }

    const Long& get_z() const noexcept
    requires(IS_SET(MASK, 2))
    {
        return m_z::value;
    }

    static Point_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Long x;
        if constexpr (IS_SET(MASK, 0)) x = Long::fetch(stream);
        Long y;
        if constexpr (IS_SET(MASK, 1)) y = Long::fetch(stream);
        Long z;
        if constexpr (IS_SET(MASK, 2)) z = Long::fetch(stream);
        Point_BASE result(std::move(x),
                          std::move(y),
                          std::move(z));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        if constexpr (IS_SET(MASK, 0)) m_x::value.store(stream);
        if constexpr (IS_SET(MASK, 1)) m_y::value.store(stream);
        if constexpr (IS_SET(MASK, 2)) m_z::value.store(stream);
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

    Point_BASE(Long&& x,
               Long&& y,
               Long&& z) noexcept
        : m_magic(MAGIC)
        , m_x(std::move(x))
        , m_y(std::move(y))
        , m_z(std::move(z))
    {}
};

using point = Point_BASE<false>;
using Point = Point_BASE<true>;

template <Nat MASK>
using point_s = Point_BASE<false, MASK>;
template <Nat MASK>
using Point_s = Point_BASE<true, MASK>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const Point_BASE<LHS_BOXED>& lhs, const Point_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_x() == rhs.get_x()
           && lhs.get_y() == rhs.get_y()
           && lhs.get_z() == rhs.get_z();
}

template <bool LHS_BOXED, bool RHS_BOXED, Nat MASK>
bool operator==(const Point_BASE<LHS_BOXED, MASK>& lhs, const Point_BASE<RHS_BOXED, MASK>& rhs) noexcept
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

}    // namespace opus::inl::simple
