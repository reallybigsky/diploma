#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::proxy::statshouse {

template <bool BOXED>
class Centroid_BASE
    : private MAYBE<Magic, BOXED, 591947165> {
private:
    using m_magic = MAYBE<Magic, BOXED, 591947165>;

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
        : m_magic(proxy, offset)
        , m_value(proxy, offset + VALUE_OFFSET)
        , m_weight(proxy, offset + WEIGHT_OFFSET)
    {}

    bool verify() const noexcept
    {
        if constexpr (BOXED)
            if (m_magic::value != MAGIC)
                return false;

        if (!get_value().verify())
            return false;

        if (!get_weight().verify())
            return false;

        return true;
    }

    Float get_value() const noexcept
    {
        return m_value;
    }

    Float get_weight() const noexcept
    {
        return m_weight;
    }

    static Centroid_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Float value = Float::fetch(stream);
        Float weight = Float::fetch(stream);
        Centroid_BASE result(std::move(value),
                             std::move(weight));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_value.store(stream);
        m_weight.store(stream);
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

    Centroid_BASE(Float&& value,
                  Float&& weight) noexcept
        : m_magic(MAGIC)
        , m_value(std::move(value))
        , m_weight(std::move(weight))
    {}

    Float m_value;
    Float m_weight;
};

using centroid = Centroid_BASE<false>;
using Centroid = Centroid_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const Centroid_BASE<LHS_BOXED>& lhs, const Centroid_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_value() == rhs.get_value()
           && lhs.get_weight() == rhs.get_weight();
}

template <bool BOXED>
size_t consume(const Centroid_BASE<BOXED>& value) noexcept
{
    size_t result = 0;
    result += consume(value.get_value());
    result += consume(value.get_weight());
    return result;
}

}    // namespace opus::proxy::statshouse
