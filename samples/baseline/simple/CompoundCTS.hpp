#pragma once

#include "prelude/baseline/Includes.hpp"

#include "samples/baseline/simple/IntDouble.hpp"
#include "samples/baseline/simple/IntDoubleLong.hpp"



namespace baseline::simple {

template <bool BOXED>
class CompoundCTS_BASE
    : private MAYBE<Magic, BOXED, 2393332885> {
private:
    using m_magic = MAYBE<Magic, BOXED, 2393332885>;

public:
    static constexpr Magic MAGIC = 2393332885;
    static constexpr offset_t SIZEOF = (BOXED ? Magic::SIZEOF : 0)
                                       + IntDouble::SIZEOF
                                       + Long::SIZEOF
                                       + intDoubleLong::SIZEOF;
    static constexpr bool STATIC = true;

    CompoundCTS_BASE() noexcept = default;

    CompoundCTS_BASE(const CompoundCTS_BASE&) noexcept = default;
    CompoundCTS_BASE(CompoundCTS_BASE&&) noexcept = default;
    CompoundCTS_BASE& operator=(const CompoundCTS_BASE&) noexcept = default;
    CompoundCTS_BASE& operator=(CompoundCTS_BASE&&) noexcept = default;

    ~CompoundCTS_BASE() noexcept = default;

    const IntDouble& get_IntDouble1() const noexcept
    {
        return m_IntDouble1;
    }

    const Long& get_Long1() const noexcept
    {
        return m_Long1;
    }

    const intDoubleLong& get_IntDoubleLong1() const noexcept
    {
        return m_IntDoubleLong1;
    }

    static CompoundCTS_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        IntDouble IntDouble1 = IntDouble::fetch(stream);
        Long Long1 = Long::fetch(stream);
        intDoubleLong IntDoubleLong1 = intDoubleLong::fetch(stream);
        CompoundCTS_BASE result(std::move(IntDouble1),
                                std::move(Long1),
                                std::move(IntDoubleLong1));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_IntDouble1.store(stream);
        m_Long1.store(stream);
        m_IntDoubleLong1.store(stream);
    }

    class Builder {
    public:
        using TYPE = CompoundCTS_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const CompoundCTS_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_IntDouble1 == rhs.get_IntDouble1()
                   && lhs.b_Long1 == rhs.get_Long1()
                   && lhs.b_IntDoubleLong1 == rhs.get_IntDoubleLong1();
        }

        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_IntDouble1(IntDouble::Builder::random(engine))
                    .set_Long1(Long::Builder::random(engine))
                    .set_IntDoubleLong1(intDoubleLong::Builder::random(engine));
        }

        Builder& set_IntDouble1(const IntDouble::Builder& value) noexcept
        {
            b_IntDouble1 = value;
            return *this;
        }

        Builder& set_Long1(const Long::Builder& value) noexcept
        {
            b_Long1 = value;
            return *this;
        }

        Builder& set_IntDoubleLong1(const intDoubleLong::Builder& value) noexcept
        {
            b_IntDoubleLong1 = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_IntDouble1.store(stream);
            b_Long1.store(stream);
            b_IntDoubleLong1.store(stream);
        }

    private:
        IntDouble::Builder b_IntDouble1;
        Long::Builder b_Long1;
        intDoubleLong::Builder b_IntDoubleLong1;
    };

private:
    CompoundCTS_BASE(IntDouble&& IntDouble1,
                     Long&& Long1,
                     intDoubleLong&& IntDoubleLong1) noexcept
        : m_magic(MAGIC)
        , m_IntDouble1(std::move(IntDouble1))
        , m_Long1(std::move(Long1))
        , m_IntDoubleLong1(std::move(IntDoubleLong1))
    {}

    IntDouble m_IntDouble1;
    Long m_Long1;
    intDoubleLong m_IntDoubleLong1;
};

using compoundCTS = CompoundCTS_BASE<false>;
using CompoundCTS = CompoundCTS_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const CompoundCTS_BASE<LHS_BOXED>& lhs, const CompoundCTS_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_IntDouble1() == rhs.get_IntDouble1()
           && lhs.get_Long1() == rhs.get_Long1()
           && lhs.get_IntDoubleLong1() == rhs.get_IntDoubleLong1();
}

}    // namespace baseline::simple
