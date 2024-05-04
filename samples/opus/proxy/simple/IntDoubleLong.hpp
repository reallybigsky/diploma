#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::proxy::simple {

template <bool BOXED>
class IntDoubleLong_BASE {
public:
    static constexpr Magic MAGIC = COMPILE_TIME_CRC32_STR("IntDoubleLong");
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
        : m_proxy(proxy, offset)
    {}

    bool verify() const noexcept
    {
        if constexpr (BOXED)
            if (*(Magic*)(m_proxy.data()) != MAGIC)
                return false;

        if (!get_Int1().verify())
            return false;

        if (!get_Double1().verify())
            return false;

        if (!get_Long1().verify())
            return false;

        return true;
    }

    Int get_Int1() const noexcept
    {
        return Int{m_proxy, INT1_OFFSET};
    }

    Double get_Double1() const noexcept
    {
        return Double{m_proxy, DOUBLE1_OFFSET};
    }

    Long get_Long1() const noexcept
    {
        return Long{m_proxy, LONG1_OFFSET};
    }

    static IntDoubleLong_BASE fetch(InputStream& stream)
    {
        Proxy proxy = Proxy::fetch(stream, SIZEOF);
        IntDoubleLong_BASE result(std::move(proxy));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_proxy.store(stream, SIZEOF);
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

    IntDoubleLong_BASE(Proxy&& proxy) noexcept
        : m_proxy(std::move(proxy))
    {}

    Proxy m_proxy;
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

};    // namespace opus::proxy::simple
