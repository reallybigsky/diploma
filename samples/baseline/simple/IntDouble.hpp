#pragma once

#include "prelude/baseline/Includes.hpp"



namespace baseline::simple {

template <bool BOXED>
class IntDouble_BASE
    : private MAYBE<Magic, BOXED, 2072402389> {
private:
    using m_magic = MAYBE<Magic, BOXED, 2072402389>;

public:
    static constexpr Magic MAGIC = 2072402389;
    static constexpr offset_t SIZEOF = (BOXED ? Magic::SIZEOF : 0)
                                       + Int::SIZEOF
                                       + Double::SIZEOF;
    static constexpr bool STATIC = true;

    IntDouble_BASE() noexcept = default;

    IntDouble_BASE(const IntDouble_BASE&) noexcept = default;
    IntDouble_BASE(IntDouble_BASE&&) noexcept = default;
    IntDouble_BASE& operator=(const IntDouble_BASE&) noexcept = default;
    IntDouble_BASE& operator=(IntDouble_BASE&&) noexcept = default;

    ~IntDouble_BASE() noexcept = default;

    const Int& get_Int1() const noexcept
    {
        return m_Int1;
    }

    const Double& get_Double1() const noexcept
    {
        return m_Double1;
    }

    static IntDouble_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Int Int1 = Int::fetch(stream);
        Double Double1 = Double::fetch(stream);
        IntDouble_BASE result(std::move(Int1),
                              std::move(Double1));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_Int1.store(stream);
        m_Double1.store(stream);
    }

    class Builder {
    public:
        using TYPE = IntDouble_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const IntDouble_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_Int1 == rhs.get_Int1()
                   && lhs.b_Double1 == rhs.get_Double1();
        }

        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_Int1(Int::Builder::random(engine))
                    .set_Double1(Double::Builder::random(engine));
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

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_Int1.store(stream);
            b_Double1.store(stream);
        }

    private:
        Int::Builder b_Int1;
        Double::Builder b_Double1;
    };

private:
    IntDouble_BASE(Int&& Int1,
                   Double&& Double1) noexcept
        : m_magic(MAGIC)
        , m_Int1(std::move(Int1))
        , m_Double1(std::move(Double1))
    {}

    Int m_Int1;
    Double m_Double1;
};

using intDouble = IntDouble_BASE<false>;
using IntDouble = IntDouble_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const IntDouble_BASE<LHS_BOXED>& lhs, const IntDouble_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_Int1() == rhs.get_Int1()
           && lhs.get_Double1() == rhs.get_Double1();
}

}    // namespace baseline::simple
