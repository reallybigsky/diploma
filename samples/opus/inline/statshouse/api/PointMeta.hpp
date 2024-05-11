#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/inline/statshouse/Dictionary.hpp"
#include "samples/opus/inline/statshouse/api/Function.hpp"



namespace opus::inl::statshouse {

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

    const Nat& get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const Long& get_time_shift() const noexcept
    {
        return m_time_shift;
    }

    const Long& get_from() const noexcept
    {
        return m_from;
    }

    const Long& get_to() const noexcept
    {
        return m_to;
    }

    const dictionary<string>& get_tags() const noexcept
    {
        return m_tags;
    }

    const std::optional<Function>& get_what() const noexcept
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
        Long time_shift = Long::fetch(stream);
        Long from = Long::fetch(stream);
        Long to = Long::fetch(stream);
        dictionary<string> tags = dictionary<string>::fetch(stream);
        std::optional<Function> what;
        if (IS_SET(fields_mask, 1)) what = Function::fetch(stream);
        PointMeta_BASE result(std::move(fields_mask),
                              std::move(time_shift),
                              std::move(from),
                              std::move(to),
                              std::move(tags),
                              std::move(what));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_fields_mask.store(stream);
        m_time_shift.store(stream);
        m_from.store(stream);
        m_to.store(stream);
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
    PointMeta_BASE(Nat&& fields_mask,
                   Long&& time_shift,
                   Long&& from,
                   Long&& to,
                   dictionary<string>&& tags,
                   std::optional<Function>&& what) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_time_shift(std::move(time_shift))
        , m_from(std::move(from))
        , m_to(std::move(to))
        , m_tags(std::move(tags))
        , m_what(std::move(what))
    {}

    Nat m_fields_mask;
    Long m_time_shift;
    Long m_from;
    Long m_to;
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

}    // namespace opus::inl::statshouse
