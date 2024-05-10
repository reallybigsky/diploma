#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::proxy_naive::statshouse {

template <bool BOXED>
class Centroid_BASE {
public:
    static constexpr Magic MAGIC = 591947165;
    static constexpr offset_t SIZEOF = (BOXED ? Magic::SIZEOF : 0)
                                       + Float::SIZEOF
                                       + Float::SIZEOF;
    static constexpr bool STATIC = true;

    Centroid_BASE() noexcept = default;

    Centroid_BASE(const Centroid_BASE&) noexcept = default;
    Centroid_BASE(Centroid_BASE&&) noexcept = default;
    Centroid_BASE& operator=(const Centroid_BASE&) noexcept = default;
    Centroid_BASE& operator=(Centroid_BASE&&) noexcept = default;

    ~Centroid_BASE() noexcept = default;

    Centroid_BASE(const Proxy& proxy, offset_t offset) noexcept
        : m_proxy(proxy, offset)
    {}

    bool verify() const noexcept
    {
        if constexpr (BOXED)
            if (*(Magic*)(m_proxy.data()) != MAGIC)
                return false;

        if (!get_value().verify())
            return false;

        if (!get_weight().verify())
            return false;

        return true;
    }

    Float get_value() const noexcept
    {
        return Float {m_proxy, VALUE_OFFSET};
    }

    Float get_weight() const noexcept
    {
        return Float {m_proxy, WEIGHT_OFFSET};
    }

    static Centroid_BASE fetch(InputStream& stream)
    {
        Proxy proxy = Proxy::fetch(stream, SIZEOF);
        Centroid_BASE result(std::move(proxy));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_proxy.store(stream, SIZEOF);
    }

    class Builder {
    public:
        using TYPE = Centroid_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const Centroid_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_value == rhs.get_value()
                   && lhs.b_weight == rhs.get_weight();
        }

        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_value(Float::Builder::random(engine))
                    .set_weight(Float::Builder::random(engine));
        }

        Builder& set_value(const Float::Builder& value) noexcept
        {
            b_value = value;
            return *this;
        }

        Builder& set_weight(const Float::Builder& value) noexcept
        {
            b_weight = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_value.store(stream);
            b_weight.store(stream);
        }

    private:
        Float::Builder b_value;
        Float::Builder b_weight;
    };

private:
    static constexpr offset_t VALUE_OFFSET = BOXED ? Magic::SIZEOF : 0;
    static constexpr offset_t WEIGHT_OFFSET = VALUE_OFFSET + Float::SIZEOF;

    Centroid_BASE(Proxy&& proxy) noexcept
        : m_proxy(std::move(proxy))
    {}

    Proxy m_proxy;
};

using centroid = Centroid_BASE<false>;
using Centroid = Centroid_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const Centroid_BASE<LHS_BOXED>& lhs, const Centroid_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_value() == rhs.get_value()
           && lhs.get_weight() == rhs.get_weight();
}

}    // namespace opus::proxy_naive::statshouse
