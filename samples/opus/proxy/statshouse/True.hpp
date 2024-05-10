#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::proxy::statshouse {

template <bool BOXED>
class True_BASE
    : private MAYBE<Magic, BOXED, 2358170397> {
private:
    using m_magic = MAYBE<Magic, BOXED, 2358170397>;

public:
    static constexpr Magic MAGIC = 2358170397;
    static constexpr offset_t SIZEOF = (BOXED ? Magic::SIZEOF : 0);
    static constexpr bool STATIC = true;

    True_BASE() noexcept
        : m_magic(MAGIC)
    {}

    True_BASE(const True_BASE&) noexcept = default;
    True_BASE(True_BASE&&) noexcept = default;
    True_BASE& operator=(const True_BASE&) noexcept = default;
    True_BASE& operator=(True_BASE&&) noexcept = default;

    ~True_BASE() noexcept = default;

    True_BASE(const Proxy& proxy, offset_t offset) noexcept
        : m_magic(proxy, offset)
    {}

    bool verify() const noexcept
    {
        if constexpr (BOXED)
            if (m_magic::value != MAGIC)
                return false;

        return true;
    }

    static True_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        True_BASE result;
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
    }

    class Builder {
    public:
        using TYPE = True_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder&, const True_BASE<RHS_BOXED>&) noexcept
        {
            return true;
        }

        static Builder random(std::default_random_engine&) noexcept
        {
            return Builder {};
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
        }

    private:
    };

private:
};

using true_t = True_BASE<false>;
using True_t = True_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const True_BASE<LHS_BOXED>&, const True_BASE<RHS_BOXED>&) noexcept
{
    return true;
}

}    // namespace opus::proxy::statshouse