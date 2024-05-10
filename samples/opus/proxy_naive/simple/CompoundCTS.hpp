#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/proxy_naive/simple/IntDouble.hpp"
#include "samples/opus/proxy_naive/simple/IntDoubleLong.hpp"



namespace opus::proxy_naive::simple {

template <bool BOXED>
class CompoundCTS_BASE {
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

    CompoundCTS_BASE(const Proxy& proxy, offset_t offset) noexcept
        : m_proxy(proxy, offset)
    {}

    bool verify() const noexcept
    {
        if constexpr (BOXED)
            if (*(Magic*)(m_proxy.data()) != MAGIC)
                return false;

        if (!get_IntDouble1().verify())
            return false;

        if (!get_Long1().verify())
            return false;

        if (!get_IntDoubleLong1().verify())
            return false;

        return true;
    }

    IntDouble get_IntDouble1() const noexcept
    {
        return IntDouble{m_proxy, INTDOUBLE1_OFFSET};
    }

    Long get_Long1() const noexcept
    {
        return Long{m_proxy, LONG1_OFFSET};
    }

    intDoubleLong get_IntDoubleLong1() const noexcept
    {
        return intDoubleLong{m_proxy, INTDOUBLELONG1_OFFSET};
    }

    static CompoundCTS_BASE fetch(InputStream& stream)
    {
        Proxy proxy = Proxy::fetch(stream, SIZEOF);
        CompoundCTS_BASE result(std::move(proxy));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_proxy.store(stream, SIZEOF);
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
    static constexpr offset_t INTDOUBLE1_OFFSET = BOXED ? Magic::SIZEOF : 0;
    static constexpr offset_t LONG1_OFFSET = INTDOUBLE1_OFFSET + IntDouble::SIZEOF;
    static constexpr offset_t INTDOUBLELONG1_OFFSET = LONG1_OFFSET + Long::SIZEOF;

    CompoundCTS_BASE(Proxy&& proxy) noexcept
        : m_proxy(std::move(proxy))
    {}

    Proxy m_proxy;
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

}    // namespace opus::proxy_naive::simple
