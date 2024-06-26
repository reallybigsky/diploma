#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/proxy/statshouse/Dictionary.hpp"
#include "samples/opus/proxy/statshouse/api/Function.hpp"



namespace opus::proxy::statshouse {

template <bool BOXED>
class PointMeta_BASE {
public:
    static constexpr Magic MAGIC = 4168453421;
    static constexpr bool STATIC = false;

    PointMeta_BASE() noexcept = default;

    PointMeta_BASE(const PointMeta_BASE&) noexcept = default;
    PointMeta_BASE(PointMeta_BASE&&) noexcept = default;
    PointMeta_BASE& operator=(const PointMeta_BASE&) noexcept = default;
    PointMeta_BASE& operator=(PointMeta_BASE&&) noexcept = default;

    ~PointMeta_BASE() noexcept = default;

    bool verify() const noexcept
    {
        if (!get_fields_mask().verify())
            return false;

        if (!get_time_shift().verify())
            return false;

        if (!get_from().verify())
            return false;

        if (!get_to().verify())
            return false;

        if (const auto& what = get_what(); what && !what->verify())
            return false;

        return true;
    }

    Nat get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    Long get_time_shift() const noexcept
    {
        return Long {m_proxy_1, PROXY_1_TIME_SHIFT_OFFSET};
    }

    Long get_from() const noexcept
    {
        return Long {m_proxy_1, PROXY_1_FROM_OFFSET};
    }

    Long get_to() const noexcept
    {
        return Long {m_proxy_1, PROXY_1_TO_OFFSET};
    }

    const dictionary<string>& get_tags() const noexcept
    {
        return m_tags;
    }

    std::optional<Function> get_what() const noexcept
    {
        return m_what;
    }

    static PointMeta_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Nat fields_mask = Nat::fetch(stream);
        Proxy proxy_1 = Proxy::fetch(stream, PROXY_1_SIZEOF);
        dictionary<string> tags = dictionary<string>::fetch(stream);
        std::optional<Function> what;
        if (IS_SET(fields_mask, 1)) what = Function::fetch(stream);
        PointMeta_BASE result(std::move(fields_mask),
                              std::move(proxy_1),
                              std::move(tags),
                              std::move(what));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_fields_mask.store(stream);
        m_proxy_1.store(stream, PROXY_1_SIZEOF);
        m_tags.store(stream);
        if (IS_SET(get_fields_mask(), 1)) m_what->store(stream);
    }

    class Builder {
    public:
        using TYPE = PointMeta_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const PointMeta_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_time_shift == rhs.get_time_shift()
                   && lhs.b_from == rhs.get_from()
                   && lhs.b_to == rhs.get_to()
                   && lhs.b_tags == rhs.get_tags()
                   && (!rhs.get_what() || lhs.b_what == rhs.get_what());
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(utils::random_mask<0, 3>(engine))
                    .set_time_shift(Long::Builder::random(engine))
                    .set_from(Long::Builder::random(engine))
                    .set_to(Long::Builder::random(engine))
                    .set_tags(dictionary<string>::Builder::random<SIZE_1, SIZE_2, SIZE_3>(engine))
                    .set_what(Function::Builder::random(engine));
        }

        Builder& set_fields_mask(const Nat::Builder& value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_time_shift(const Long::Builder& value) noexcept
        {
            b_time_shift = value;
            return *this;
        }

        Builder& set_from(const Long::Builder& value) noexcept
        {
            b_from = value;
            return *this;
        }

        Builder& set_to(const Long::Builder& value) noexcept
        {
            b_to = value;
            return *this;
        }

        Builder& set_tags(const dictionary<string>::Builder& value) noexcept
        {
            b_tags = value;
            return *this;
        }

        Builder& set_what(const Function::Builder& value) noexcept
        {
            b_what = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_fields_mask.store(stream);
            b_time_shift.store(stream);
            b_from.store(stream);
            b_to.store(stream);
            b_tags.store(stream);
            if (IS_SET(b_fields_mask, 1)) b_what.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        Long::Builder b_time_shift;
        Long::Builder b_from;
        Long::Builder b_to;
        dictionary<string>::Builder b_tags;
        Function::Builder b_what;
    };

private:
    static constexpr offset_t PROXY_1_SIZEOF = Long::SIZEOF
                                               + Long::SIZEOF
                                               + Long::SIZEOF;
    static constexpr offset_t PROXY_1_TIME_SHIFT_OFFSET = 0;
    static constexpr offset_t PROXY_1_FROM_OFFSET = PROXY_1_TIME_SHIFT_OFFSET + Long::SIZEOF;
    static constexpr offset_t PROXY_1_TO_OFFSET = PROXY_1_FROM_OFFSET + Long::SIZEOF;


    PointMeta_BASE(Nat&& fields_mask,
                   Proxy&& proxy_1,
                   dictionary<string>&& tags,
                   std::optional<Function>&& what) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_proxy_1(std::move(proxy_1))
        , m_tags(std::move(tags))
        , m_what(std::move(what))
    {}

    Nat m_fields_mask;
    Proxy m_proxy_1;
    dictionary<string> m_tags;
    std::optional<Function> m_what;
};

using pointMeta = PointMeta_BASE<false>;
using PointMeta = PointMeta_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const PointMeta_BASE<LHS_BOXED>& lhs, const PointMeta_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_time_shift() == rhs.get_time_shift()
           && lhs.get_from() == rhs.get_from()
           && lhs.get_to() == rhs.get_to()
           && lhs.get_tags() == rhs.get_tags()
           && lhs.get_what() == rhs.get_what();
}

template <bool BOXED>
size_t consume(const PointMeta_BASE<BOXED>& value) noexcept
{
    size_t result = 0;
    result += consume(value.get_fields_mask());
    result += consume(value.get_time_shift());
    result += consume(value.get_from());
    result += consume(value.get_to());
    result += consume(value.get_tags());
    if (value.get_what()) result += consume(*value.get_what());
    return result;
}

}    // namespace opus::proxy::statshouse
