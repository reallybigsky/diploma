#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::proxy_naive::simple {

template <bool BOXED>
class StringIntLong_BASE {
public:
    static constexpr Magic MAGIC = COMPILE_TIME_CRC32_STR("StringIntLong");
    static constexpr bool STATIC = false;

    StringIntLong_BASE() noexcept = default;

    StringIntLong_BASE(const StringIntLong_BASE&) noexcept = default;
    StringIntLong_BASE(StringIntLong_BASE&&) noexcept = default;
    StringIntLong_BASE& operator=(const StringIntLong_BASE&) noexcept = default;
    StringIntLong_BASE& operator=(StringIntLong_BASE&&) noexcept = default;

    ~StringIntLong_BASE() noexcept = default;

    bool verify() const noexcept
    {
        if (!get_Int1().verify())
            return false;

        if (!get_Long1().verify())
            return false;

        return true;
    }

    const String& get_String1() const noexcept
    {
        return m_String1;
    }

    Int get_Int1() const noexcept
    {
        return Int {m_proxy_1, PROXY_1_INT1_OFFSET};
    }

    Long get_Long1() const noexcept
    {
        return Long {m_proxy_1, PROXY_1_LONG1_OFFSET};
    }

    static StringIntLong_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        String String1 = String::fetch(stream);
        Proxy proxy_1 = Proxy::fetch(stream, PROXY_1_SIZEOF);
        StringIntLong_BASE result(std::move(String1),
                                  std::move(proxy_1));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_String1.store(stream);
        m_proxy_1.store(stream, PROXY_1_SIZEOF);
    }

    class Builder {
    public:
        using TYPE = StringIntLong_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const StringIntLong_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_String1 == rhs.get_String1()
                   && lhs.b_Int1 == rhs.get_Int1()
                   && lhs.b_Long1 == rhs.get_Long1();
        }

        template <size_t SIZE>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_String1(String::Builder::random<SIZE>(engine))
                    .set_Int1(Int::Builder::random(engine))
                    .set_Long1(Long::Builder::random(engine));
        }

        Builder& set_String1(const String::Builder& value) noexcept
        {
            b_String1 = value;
            return *this;
        }

        Builder& set_Int1(const Int::Builder& value) noexcept
        {
            b_Int1 = value;
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
            b_String1.store(stream);
            b_Int1.store(stream);
            b_Long1.store(stream);
        }

    private:
        String::Builder b_String1;
        Int::Builder b_Int1;
        Long::Builder b_Long1;
    };

private:
    static constexpr offset_t PROXY_1_SIZEOF = Int::SIZEOF
                                               + Long::SIZEOF;
    static constexpr offset_t PROXY_1_INT1_OFFSET = 0;
    static constexpr offset_t PROXY_1_LONG1_OFFSET = PROXY_1_INT1_OFFSET + Int::SIZEOF;

    StringIntLong_BASE(String&& String1,
                       Proxy&& proxy_1) noexcept
        : m_String1(std::move(String1))
        , m_proxy_1(std::move(proxy_1))
    {}

    String m_String1;
    Proxy m_proxy_1;
};
using stringIntLong = StringIntLong_BASE<false>;
using StringIntLong = StringIntLong_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const StringIntLong_BASE<LHS_BOXED>& lhs, const StringIntLong_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_String1() == rhs.get_String1()
           && lhs.get_Int1() == rhs.get_Int1()
           && lhs.get_Long1() == rhs.get_Long1();
}

}    // namespace opus::proxy_naive::simple
