#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/inline/statshouse/schema/MultiValue.hpp"
#include "samples/opus/inline/statshouse/schema/TopElement.hpp"



namespace opus::inl::statshouse {

template <bool BOXED>
class MultiItem_BASE {
public:
    static constexpr Magic MAGIC = 3048832961;
    static constexpr bool STATIC = false;

    MultiItem_BASE() noexcept = default;

    MultiItem_BASE(const MultiItem_BASE&) noexcept = default;
    MultiItem_BASE(MultiItem_BASE&&) noexcept = default;
    MultiItem_BASE& operator=(const MultiItem_BASE&) noexcept = default;
    MultiItem_BASE& operator=(MultiItem_BASE&&) noexcept = default;

    ~MultiItem_BASE() noexcept = default;

    const Nat& get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const Int& get_metric() const noexcept
    {
        return m_metric;
    }

    const array<Int>& get_keys() const noexcept
    {
        return m_keys;
    }

    const std::optional<Nat>& get_t() const noexcept
    {
        return m_t;
    }

    const multiValue& get_tail() const noexcept
    {
        return m_tail;
    }

    const std::optional<array<topElement>>& get_top() const noexcept
    {
        return m_top;
    }

    static MultiItem_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Nat fields_mask = Nat::fetch(stream);
        Int metric = Int::fetch(stream);
        array<Int> keys = array<Int>::fetch(stream);
        std::optional<Nat> t;
        if (IS_SET(fields_mask, 10)) t = Nat::fetch(stream);
        multiValue tail = multiValue::fetch(stream, fields_mask);
        std::optional<array<topElement>> top;
        if (IS_SET(fields_mask, 11)) top = array<topElement>::fetch(stream);
        MultiItem_BASE result(std::move(fields_mask),
                              std::move(metric),
                              std::move(keys),
                              std::move(t),
                              std::move(tail),
                              std::move(top));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_fields_mask.store(stream);
        m_metric.store(stream);
        m_keys.store(stream);
        if (IS_SET(get_fields_mask(), 10)) m_t->store(stream);
        m_tail.store(stream, get_fields_mask());
        if (IS_SET(get_fields_mask(), 11)) m_top->store(stream);
    }

    class Builder {
    public:
        using TYPE = MultiItem_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const MultiItem_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_metric == rhs.get_metric()
                   && lhs.b_keys == rhs.get_keys()
                   && (!rhs.get_t() || lhs.b_t == rhs.get_t())
                   && lhs.b_tail == rhs.get_tail()
                   && (!rhs.get_top() || lhs.b_top == rhs.get_top());
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3, size_t SIZE_4, size_t SIZE_5, size_t SIZE_6, size_t SIZE_7>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(utils::random_mask<0, 4095>(engine))
                    .set_metric(Int::Builder::random(engine))
                    .set_keys(array<Int>::Builder::random<SIZE_1>(engine))
                    .set_t(Nat::Builder::random(engine))
                    .set_tail(multiValue::Builder::random<SIZE_2, SIZE_3>(engine))
                    .set_top(array<topElement>::Builder::random<SIZE_4, SIZE_5, SIZE_6, SIZE_7>(engine));
        }

        Builder& set_fields_mask(const Nat::Builder& value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_metric(const Int::Builder& value) noexcept
        {
            b_metric = value;
            return *this;
        }

        Builder& set_keys(const array<Int>::Builder& value) noexcept
        {
            b_keys = value;
            return *this;
        }

        Builder& set_t(const Nat::Builder& value) noexcept
        {
            b_t = value;
            return *this;
        }

        Builder& set_tail(const multiValue::Builder& value) noexcept
        {
            b_tail = value;
            return *this;
        }

        Builder& set_top(const array<topElement>::Builder& value) noexcept
        {
            b_top = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_fields_mask.store(stream);
            b_metric.store(stream);
            b_keys.store(stream);
            if (IS_SET(b_fields_mask, 10)) b_t.store(stream);
            b_tail.store(stream, b_fields_mask);
            if (IS_SET(b_fields_mask, 11)) b_top.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        Int::Builder b_metric;
        array<Int>::Builder b_keys;
        Nat::Builder b_t;
        multiValue::Builder b_tail;
        array<topElement>::Builder b_top;
    };

private:
    MultiItem_BASE(Nat&& fields_mask,
                   Int&& metric,
                   array<Int>&& keys,
                   std::optional<Nat>&& t,
                   multiValue&& tail,
                   std::optional<array<topElement>>&& top) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_metric(std::move(metric))
        , m_keys(std::move(keys))
        , m_t(std::move(t))
        , m_tail(std::move(tail))
        , m_top(std::move(top))
    {}

    Nat m_fields_mask;
    Int m_metric;
    array<Int> m_keys;
    std::optional<Nat> m_t;
    multiValue m_tail;
    std::optional<array<topElement>> m_top;
};

using multiItem = MultiItem_BASE<false>;
using MultiItem = MultiItem_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const MultiItem_BASE<LHS_BOXED>& lhs, const MultiItem_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_metric() == rhs.get_metric()
           && lhs.get_keys() == rhs.get_keys()
           && lhs.get_t() == rhs.get_t()
           && lhs.get_tail() == rhs.get_tail()
           && lhs.get_top() == rhs.get_top();
}

template <bool BOXED>
size_t consume(const MultiItem_BASE<BOXED>& value) noexcept
{
    size_t result = 0;
    result += consume(value.get_fields_mask());
    result += consume(value.get_metric());
    result += consume(value.get_keys());
    if (value.get_t()) result += consume(*value.get_t());
    result += consume(value.get_tail());
    if (value.get_top()) result += consume(*value.get_top());
    return result;
}

}    // namespace opus::inl::statshouse
