#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::inl::simple {

template <bool BOXED>
class IntDoubleLong_BASE
    : private MAYBE<Magic, BOXED, 2222193245> {
private:
    using m_magic = MAYBE<Magic, BOXED, 2222193245>;

public:
    static constexpr Magic MAGIC = 2222193245;
    static constexpr offset_t SIZEOF = (BOXED ? Magic::SIZEOF : 0)
                                       + Int::SIZEOF
                                       + Double::SIZEOF
                                       + Long::SIZEOF;
    static constexpr bool STATIC = true;

    IntDoubleLong_BASE() noexcept = default;

    IntDoubleLong_BASE(const IntDoubleLong_BASE&) noexcept = default;
    IntDoubleLong_BASE(IntDoubleLong_BASE&&) noexcept = default;
    IntDoubleLong_BASE& operator=(const IntDoubleLong_BASE&) noexcept = default;
    IntDoubleLong_BASE& operator=(IntDoubleLong_BASE&&) noexcept = default;

    ~IntDoubleLong_BASE() noexcept = default;

    IntDoubleLong_BASE(const Proxy& proxy, offset_t offset) noexcept
        : m_magic(proxy, offset)
        , m_Int1(proxy, offset + INT1_OFFSET)
        , m_Double1(proxy, offset + DOUBLE1_OFFSET)
        , m_Long1(proxy, offset + LONG1_OFFSET)
    {}

    bool verify() const noexcept
    {
        if constexpr (BOXED)
            if (m_magic::value != MAGIC)
                return false;

        if (!get_Int1().verify())
            return false;

        if (!get_Double1().verify())
            return false;

        if (!get_Long1().verify())
            return false;

        return true;
    }

    const Int& get_Int1() const noexcept
    {
        return m_Int1;
    }

    const Double& get_Double1() const noexcept
    {
        return m_Double1;
    }

    const Long& get_Long1() const noexcept
    {
        return m_Long1;
    }

    static IntDoubleLong_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Int Int1 = Int::fetch(stream);
        Double Double1 = Double::fetch(stream);
        Long Long1 = Long::fetch(stream);
        IntDoubleLong_BASE result(std::move(Int1),
                                  std::move(Double1),
                                  std::move(Long1));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_Int1.store(stream);
        m_Double1.store(stream);
        m_Long1.store(stream);
    }

    class Builder {
    public:
        using TYPE = IntDoubleLong_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const IntDoubleLong_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_Int1 == rhs.get_Int1()
                   && lhs.b_Double1 == rhs.get_Double1()
                   && lhs.b_Long1 == rhs.get_Long1();
        }

        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_Int1(Int::Builder::random(engine))
                    .set_Double1(Double::Builder::random(engine))
                    .set_Long1(Long::Builder::random(engine));
        }

        Builder& set_Int1(const Int::Builder& value) noexcept
        {
            b_Int1 = value;
            return *this;
        }

        Builder& set_Double1(const Double::Builder& value) noexcept
        {
            b_Double1 = value;
            return *this;
        }

        Builder& set_Long1(const Long::Builder& value) noexcept
        {
            b_Long1 = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_Int1.store(stream);
            b_Double1.store(stream);
            b_Long1.store(stream);
        }

    private:
        Int::Builder b_Int1;
        Double::Builder b_Double1;
        Long::Builder b_Long1;
    };

private:
    static constexpr offset_t INT1_OFFSET = BOXED ? Magic::SIZEOF : 0;
    static constexpr offset_t DOUBLE1_OFFSET = INT1_OFFSET + Int::SIZEOF;
    static constexpr offset_t LONG1_OFFSET = DOUBLE1_OFFSET + Double::SIZEOF;

    IntDoubleLong_BASE(Int&& Int1,
                       Double&& Double1,
                       Long&& Long1) noexcept
        : m_magic(MAGIC)
        , m_Int1(std::move(Int1))
        , m_Double1(std::move(Double1))
        , m_Long1(std::move(Long1))
    {}

    Int m_Int1;
    Double m_Double1;
    Long m_Long1;
};

using intDoubleLong = IntDoubleLong_BASE<false>;
using IntDoubleLong = IntDoubleLong_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const IntDoubleLong_BASE<LHS_BOXED>& lhs, const IntDoubleLong_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_Int1() == rhs.get_Int1()
           && lhs.get_Double1() == rhs.get_Double1()
           && lhs.get_Long1() == rhs.get_Long1();
}

};    // namespace opus::inl::simple
