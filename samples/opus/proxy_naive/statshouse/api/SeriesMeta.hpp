#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/proxy_naive/statshouse/Dictionary.hpp"
#include "samples/opus/proxy_naive/statshouse/api/Function.hpp"



namespace opus::proxy_naive::statshouse {

template <bool BOXED, Nat...>
class SeriesMeta_BASE;

template <bool BOXED>
class SeriesMeta_BASE<BOXED> {
public:
    static constexpr Magic MAGIC = 3280544510;
    static constexpr bool STATIC = false;

    SeriesMeta_BASE() noexcept = default;

    SeriesMeta_BASE(const SeriesMeta_BASE&) noexcept = default;
    SeriesMeta_BASE(SeriesMeta_BASE&&) noexcept = default;
    SeriesMeta_BASE& operator=(const SeriesMeta_BASE&) noexcept = default;
    SeriesMeta_BASE& operator=(SeriesMeta_BASE&&) noexcept = default;

    ~SeriesMeta_BASE() noexcept = default;

    bool verify() const noexcept
    {
        if (!get_fields_mask().verify())
            return false;

        if (!get_time_shift().verify())
            return false;

        if (const auto& what = get_what(); what && !what->verify())
            return false;

        if (const auto& total = get_total(); total && !total->verify())
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

    const dictionary<string>& get_tags() const noexcept
    {
        return m_tags;
    }

    std::optional<Function> get_what() const noexcept
    {
        return PROXY_2_WHAT_OFFSET == NO_VALUE ? std::optional<Function>() : Function {m_proxy_2, PROXY_2_WHAT_OFFSET};
    }

    const std::optional<string>& get_name() const noexcept
    {
        return m_name;
    }

    const std::optional<string>& get_color() const noexcept
    {
        return m_color;
    }

    std::optional<Int> get_total() const noexcept
    {
        return PROXY_3_TOTAL_OFFSET == NO_VALUE ? std::optional<Int>() : Int {m_proxy_3, PROXY_3_TOTAL_OFFSET};
    }

    const std::optional<array<string>>& get_max_hosts() const noexcept
    {
        return m_max_hosts;
    }

    static SeriesMeta_BASE fetch(InputStream& stream, Nat query_fields_mask)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Nat fields_mask = Nat::fetch(stream);
        Proxy proxy_1 = Proxy::fetch(stream, PROXY_1_SIZEOF);
        dictionary<string> tags = dictionary<string>::fetch(stream);
        offset_t PROXY_2_SIZEOF = (IS_SET(fields_mask, 1) ? Function::SIZEOF : 0);
        offset_t proxy_2_pos = 0;
        offset_t PROXY_2_WHAT_OFFSET = NO_VALUE;
        if (IS_SET(fields_mask, 1)) {
            PROXY_2_WHAT_OFFSET = proxy_2_pos;
            proxy_2_pos += Function::SIZEOF;
        }
        Proxy proxy_2 = Proxy::fetch(stream, PROXY_2_SIZEOF);
        std::optional<string> name;
        if (IS_SET(query_fields_mask, 4)) name = string::fetch(stream);
        std::optional<string> color;
        if (IS_SET(query_fields_mask, 5)) color = string::fetch(stream);
        offset_t PROXY_3_SIZEOF = (IS_SET(query_fields_mask, 6) ? Int::SIZEOF : 0);
        offset_t proxy_3_pos = 0;
        offset_t PROXY_3_TOTAL_OFFSET = NO_VALUE;
        if (IS_SET(query_fields_mask, 6)) {
            PROXY_3_TOTAL_OFFSET = proxy_3_pos;
            proxy_3_pos += Int::SIZEOF;
        }
        Proxy proxy_3 = Proxy::fetch(stream, PROXY_3_SIZEOF);
        std::optional<array<string>> max_hosts;
        if (IS_SET(query_fields_mask, 7)) max_hosts = array<string>::fetch(stream);
        SeriesMeta_BASE result(std::move(fields_mask),
                               std::move(proxy_1),
                               std::move(tags),
                               std::move(PROXY_2_SIZEOF),
                               std::move(PROXY_2_WHAT_OFFSET),
                               std::move(proxy_2),
                               std::move(name),
                               std::move(color),
                               std::move(PROXY_3_SIZEOF),
                               std::move(PROXY_3_TOTAL_OFFSET),
                               std::move(proxy_3),
                               std::move(max_hosts));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream, Nat query_fields_mask) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_fields_mask.store(stream);
        m_proxy_1.store(stream, PROXY_1_SIZEOF);
        m_tags.store(stream);
        m_proxy_2.store(stream, PROXY_2_SIZEOF);
        if (IS_SET(query_fields_mask, 4)) m_name->store(stream);
        if (IS_SET(query_fields_mask, 5)) m_color->store(stream);
        m_proxy_3.store(stream, PROXY_3_SIZEOF);
        if (IS_SET(query_fields_mask, 7)) m_max_hosts->store(stream);
    }

    class Builder {
    public:
        using TYPE = SeriesMeta_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const SeriesMeta_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_time_shift == rhs.get_time_shift()
                   && lhs.b_tags == rhs.get_tags()
                   && (!rhs.get_what() || lhs.b_what == rhs.get_what())
                   && (!rhs.get_name() || lhs.b_name == rhs.get_name())
                   && (!rhs.get_color() || lhs.b_color == rhs.get_color())
                   && (!rhs.get_total() || lhs.b_total == rhs.get_total())
                   && (!rhs.get_max_hosts() || lhs.b_max_hosts == rhs.get_max_hosts());
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3, size_t SIZE_4, size_t SIZE_5, size_t SIZE_6, size_t SIZE_7>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(utils::random_mask<0, 3>(engine))
                    .set_time_shift(Long::Builder::random(engine))
                    .set_tags(dictionary<string>::Builder::random<SIZE_1, SIZE_2, SIZE_3>(engine))
                    .set_what(Function::Builder::random(engine))
                    .set_name(string::Builder::random<SIZE_4>(engine))
                    .set_color(string::Builder::random<SIZE_5>(engine))
                    .set_total(Int::Builder::random(engine))
                    .set_max_hosts(array<string>::Builder::random<SIZE_6, SIZE_7>(engine));
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

        Builder& set_name(const string::Builder& value) noexcept
        {
            b_name = value;
            return *this;
        }

        Builder& set_color(const string::Builder& value) noexcept
        {
            b_color = value;
            return *this;
        }

        Builder& set_total(const Int::Builder& value) noexcept
        {
            b_total = value;
            return *this;
        }

        Builder& set_max_hosts(const array<string>::Builder& value) noexcept
        {
            b_max_hosts = value;
            return *this;
        }

        void store(OutputStream& stream, Nat query_fields_mask) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_fields_mask.store(stream);
            b_time_shift.store(stream);
            b_tags.store(stream);
            if (IS_SET(b_fields_mask, 1)) b_what.store(stream);
            if (IS_SET(query_fields_mask, 4)) b_name.store(stream);
            if (IS_SET(query_fields_mask, 5)) b_color.store(stream);
            if (IS_SET(query_fields_mask, 6)) b_total.store(stream);
            if (IS_SET(query_fields_mask, 7)) b_max_hosts.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        Long::Builder b_time_shift;
        dictionary<string>::Builder b_tags;
        Function::Builder b_what;
        string::Builder b_name;
        string::Builder b_color;
        Int::Builder b_total;
        array<string>::Builder b_max_hosts;
    };

private:
    static constexpr offset_t PROXY_1_SIZEOF = Long::SIZEOF;
    static constexpr offset_t PROXY_1_TIME_SHIFT_OFFSET = 0;

    SeriesMeta_BASE(Nat&& fields_mask,
                    Proxy&& proxy_1,
                    dictionary<string>&& tags,
                    offset_t&& PROXY_2_SIZEOF,
                    offset_t&& PROXY_2_WHAT_OFFSET,
                    Proxy&& proxy_2,
                    std::optional<string>&& name,
                    std::optional<string>&& color,
                    offset_t&& PROXY_3_SIZEOF,
                    offset_t&& PROXY_3_TOTAL_OFFSET,
                    Proxy&& proxy_3,
                    std::optional<array<string>>&& max_hosts) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_proxy_1(std::move(proxy_1))
        , m_tags(std::move(tags))
        , PROXY_2_SIZEOF(std::move(PROXY_2_SIZEOF))
        , PROXY_2_WHAT_OFFSET(std::move(PROXY_2_WHAT_OFFSET))
        , m_proxy_2(std::move(proxy_2))
        , m_name(std::move(name))
        , m_color(std::move(color))
        , PROXY_3_SIZEOF(std::move(PROXY_3_SIZEOF))
        , PROXY_3_TOTAL_OFFSET(std::move(PROXY_3_TOTAL_OFFSET))
        , m_proxy_3(std::move(proxy_3))
        , m_max_hosts(std::move(max_hosts))
    {}

    Nat m_fields_mask;
    Proxy m_proxy_1;
    dictionary<string> m_tags;
    offset_t PROXY_2_SIZEOF = 0;
    offset_t PROXY_2_WHAT_OFFSET = NO_VALUE;
    Proxy m_proxy_2;
    std::optional<string> m_name;
    std::optional<string> m_color;
    offset_t PROXY_3_SIZEOF = 0;
    offset_t PROXY_3_TOTAL_OFFSET = NO_VALUE;
    Proxy m_proxy_3;
    std::optional<array<string>> m_max_hosts;
};

template <bool BOXED, Nat QUERY_FIELDS_MASK>
class SeriesMeta_BASE<BOXED, QUERY_FIELDS_MASK>
    : private MAYBE<string, IS_SET(QUERY_FIELDS_MASK, 4), utils::commutative(3280544510, 4)>
    , private MAYBE<string, IS_SET(QUERY_FIELDS_MASK, 5), utils::commutative(3280544510, 5)>
    , private MAYBE<Int, IS_SET(QUERY_FIELDS_MASK, 6), utils::commutative(3280544510, 6)>
    , private MAYBE<array<string>, IS_SET(QUERY_FIELDS_MASK, 7), utils::commutative(3280544510, 7)> {
private:
    using m_name = MAYBE<string, IS_SET(QUERY_FIELDS_MASK, 4), utils::commutative(3280544510, 4)>;
    using m_color = MAYBE<string, IS_SET(QUERY_FIELDS_MASK, 5), utils::commutative(3280544510, 5)>;
    using m_total = MAYBE<Int, IS_SET(QUERY_FIELDS_MASK, 6), utils::commutative(3280544510, 6)>;
    using m_max_hosts = MAYBE<array<string>, IS_SET(QUERY_FIELDS_MASK, 7), utils::commutative(3280544510, 7)>;

public:
    static constexpr Magic MAGIC = 3280544510;
    static constexpr bool STATIC = false;

    SeriesMeta_BASE() noexcept = default;

    SeriesMeta_BASE(const SeriesMeta_BASE&) noexcept = default;
    SeriesMeta_BASE(SeriesMeta_BASE&&) noexcept = default;
    SeriesMeta_BASE& operator=(const SeriesMeta_BASE&) noexcept = default;
    SeriesMeta_BASE& operator=(SeriesMeta_BASE&&) noexcept = default;

    ~SeriesMeta_BASE() noexcept = default;

    bool verify() const noexcept
    {
        if (!get_fields_mask().verify())
            return false;

        if (!get_time_shift().verify())
            return false;

        if (const auto& what = get_what(); what && !what->verify())
            return false;

        if constexpr (IS_SET(QUERY_FIELDS_MASK, 6))
            if (!get_total().verify())
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

    const dictionary<string>& get_tags() const noexcept
    {
        return m_tags;
    }

    std::optional<Function> get_what() const noexcept
    {
        return PROXY_2_WHAT_OFFSET == NO_VALUE ? std::optional<Function>() : Function {m_proxy_2, PROXY_2_WHAT_OFFSET};
    }

    const string& get_name() const noexcept
    requires(IS_SET(QUERY_FIELDS_MASK, 4))
    {
        return m_name::value;
    }

    const string& get_color() const noexcept
    requires(IS_SET(QUERY_FIELDS_MASK, 5))
    {
        return m_color::value;
    }

    Int get_total() const noexcept
    requires(IS_SET(QUERY_FIELDS_MASK, 6))
    {
        return m_total::value;
    }

    const array<string>& get_max_hosts() const noexcept
    requires(IS_SET(QUERY_FIELDS_MASK, 7))
    {
        return m_max_hosts::value;
    }

    static SeriesMeta_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Nat fields_mask = Nat::fetch(stream);
        Proxy proxy_1 = Proxy::fetch(stream, PROXY_1_SIZEOF);
        dictionary<string> tags = dictionary<string>::fetch(stream);
        offset_t PROXY_2_SIZEOF = (IS_SET(fields_mask, 1) ? Function::SIZEOF : 0);
        offset_t proxy_2_pos = 0;
        offset_t PROXY_2_WHAT_OFFSET = NO_VALUE;
        if (IS_SET(fields_mask, 1)) {
            PROXY_2_WHAT_OFFSET = proxy_2_pos;
            proxy_2_pos += Function::SIZEOF;
        }
        Proxy proxy_2 = Proxy::fetch(stream, PROXY_2_SIZEOF);
        string name;
        if constexpr (IS_SET(QUERY_FIELDS_MASK, 4)) name = string::fetch(stream);
        string color;
        if constexpr (IS_SET(QUERY_FIELDS_MASK, 5)) color = string::fetch(stream);
        Int total;
        if constexpr (IS_SET(QUERY_FIELDS_MASK, 6)) total = Int::fetch(stream);
        array<string> max_hosts;
        if constexpr (IS_SET(QUERY_FIELDS_MASK, 7)) max_hosts = array<string>::fetch(stream);
        SeriesMeta_BASE result(std::move(fields_mask),
                               std::move(proxy_1),
                               std::move(tags),
                               std::move(PROXY_2_SIZEOF),
                               std::move(PROXY_2_WHAT_OFFSET),
                               std::move(proxy_2),
                               std::move(name),
                               std::move(color),
                               std::move(total),
                               std::move(max_hosts));
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
        m_proxy_2.store(stream, PROXY_2_SIZEOF);
        if constexpr (IS_SET(QUERY_FIELDS_MASK, 4)) m_name::value.store(stream);
        if constexpr (IS_SET(QUERY_FIELDS_MASK, 5)) m_color::value.store(stream);
        if constexpr (IS_SET(QUERY_FIELDS_MASK, 6)) m_total::value.store(stream);
        if constexpr (IS_SET(QUERY_FIELDS_MASK, 7)) m_max_hosts::value.store(stream);
    }

    class Builder
        : private MAYBE<string::Builder, IS_SET(QUERY_FIELDS_MASK, 4), utils::commutative(3280544510, 4)>
        , private MAYBE<string::Builder, IS_SET(QUERY_FIELDS_MASK, 5), utils::commutative(3280544510, 5)>
        , private MAYBE<Int::Builder, IS_SET(QUERY_FIELDS_MASK, 6), utils::commutative(3280544510, 6)>
        , private MAYBE<array<string>::Builder, IS_SET(QUERY_FIELDS_MASK, 7), utils::commutative(3280544510, 7)> {
    private:
        using b_name = MAYBE<string::Builder, IS_SET(QUERY_FIELDS_MASK, 4), utils::commutative(3280544510, 4)>;
        using b_color = MAYBE<string::Builder, IS_SET(QUERY_FIELDS_MASK, 5), utils::commutative(3280544510, 5)>;
        using b_total = MAYBE<Int::Builder, IS_SET(QUERY_FIELDS_MASK, 6), utils::commutative(3280544510, 6)>;
        using b_max_hosts = MAYBE<array<string>::Builder, IS_SET(QUERY_FIELDS_MASK, 7), utils::commutative(3280544510, 7)>;

    public:
        using TYPE = SeriesMeta_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const SeriesMeta_BASE<RHS_BOXED, QUERY_FIELDS_MASK>& rhs) noexcept
        {
            if (lhs.b_fields_mask != rhs.get_fields_mask())
                return false;

            if (lhs.b_time_shift != rhs.get_time_shift())
                return false;

            if (lhs.b_tags != rhs.get_tags())
                return false;

            if (rhs.get_what() && lhs.b_what != rhs.get_what())
                return false;

            if constexpr (IS_SET(QUERY_FIELDS_MASK, 4))
                if (lhs.b_name::value != rhs.get_name())
                    return false;

            if constexpr (IS_SET(QUERY_FIELDS_MASK, 5))
                if (lhs.b_color::value != rhs.get_color())
                    return false;

            if constexpr (IS_SET(QUERY_FIELDS_MASK, 6))
                if (lhs.b_total::value != rhs.get_total())
                    return false;

            if constexpr (IS_SET(QUERY_FIELDS_MASK, 7))
                if (lhs.b_max_hosts::value != rhs.get_max_hosts())
                    return false;

            return true;
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

        Builder& set_name(const string::Builder& value) noexcept
        requires(IS_SET(QUERY_FIELDS_MASK, 4))
        {
            b_name::value = value;
            return *this;
        }

        Builder& set_color(const string::Builder& value) noexcept
        requires(IS_SET(QUERY_FIELDS_MASK, 5))
        {
            b_color::value = value;
            return *this;
        }

        Builder& set_total(Int::Builder value) noexcept
        requires(IS_SET(QUERY_FIELDS_MASK, 6))
        {
            b_total::value = value;
            return *this;
        }

        Builder& set_max_hosts(const array<string>::Builder& value) noexcept
        requires(IS_SET(QUERY_FIELDS_MASK, 7))
        {
            b_max_hosts::value = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_fields_mask.store(stream);
            b_time_shift.store(stream);
            b_tags.store(stream);
            if (IS_SET(b_fields_mask, 1)) b_what.store(stream);
            if constexpr (IS_SET(QUERY_FIELDS_MASK, 4)) b_name::value.store(stream);
            if constexpr (IS_SET(QUERY_FIELDS_MASK, 5)) b_color::value.store(stream);
            if constexpr (IS_SET(QUERY_FIELDS_MASK, 6)) b_total::value.store(stream);
            if constexpr (IS_SET(QUERY_FIELDS_MASK, 7)) b_max_hosts::value.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        Long::Builder b_time_shift;
        dictionary<string>::Builder b_tags;
        Function::Builder b_what;
    };

private:
    static constexpr offset_t PROXY_1_SIZEOF = Long::SIZEOF;
    static constexpr offset_t PROXY_1_TIME_SHIFT_OFFSET = 0;

    SeriesMeta_BASE(Nat&& fields_mask,
                    Proxy&& proxy_1,
                    dictionary<string>&& tags,
                    offset_t&& PROXY_2_SIZEOF,
                    offset_t&& PROXY_2_WHAT_OFFSET,
                    Proxy&& proxy_2,
                    std::optional<string>&& name,
                    std::optional<string>&& color,
                    Int&& total,
                    array<string>&& max_hosts) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_proxy_1(std::move(proxy_1))
        , m_tags(std::move(tags))
        , PROXY_2_SIZEOF(std::move(PROXY_2_SIZEOF))
        , m_proxy_2(std::move(proxy_2))
        , PROXY_2_WHAT_OFFSET(std::move(PROXY_2_WHAT_OFFSET))
        , m_name(std::move(name))
        , m_color(std::move(color))
        , m_total(std::move(total))
        , m_max_hosts(std::move(max_hosts))
    {}

    Nat m_fields_mask;
    Proxy m_proxy_1;
    dictionary<string> m_tags;
    offset_t PROXY_2_SIZEOF;
    offset_t PROXY_2_WHAT_OFFSET = NO_VALUE;
    Proxy m_proxy_2;
};

using seriesMeta = SeriesMeta_BASE<false>;
using SeriesMeta = SeriesMeta_BASE<true>;

template <Nat QUERY_FIELDS_MASK>
using seriesMeta_s = SeriesMeta_BASE<false, QUERY_FIELDS_MASK>;
template <Nat QUERY_FIELDS_MASK>
using SeriesMeta_s = SeriesMeta_BASE<true, QUERY_FIELDS_MASK>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const SeriesMeta_BASE<LHS_BOXED>& lhs, const SeriesMeta_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_time_shift() == rhs.get_time_shift()
           && lhs.get_tags() == rhs.get_tags()
           && lhs.get_what() == rhs.get_what()
           && lhs.get_name() == rhs.get_name()
           && lhs.get_color() == rhs.get_color()
           && lhs.get_total() == rhs.get_total()
           && lhs.get_max_hosts() == rhs.get_max_hosts();
}

template <bool LHS_BOXED, bool RHS_BOXED, Nat QUERY_FIELDS_MASK>
bool operator==(const SeriesMeta_BASE<LHS_BOXED, QUERY_FIELDS_MASK>& lhs, const SeriesMeta_BASE<RHS_BOXED, QUERY_FIELDS_MASK>& rhs) noexcept
{
    if (lhs.get_fields_mask() != rhs.get_fields_mask())
        return false;

    if (lhs.get_time_shift() != rhs.get_time_shift())
        return false;

    if (lhs.get_tags() != rhs.get_tags())
        return false;

    if (lhs.get_what() != rhs.get_what())
        return false;

    if constexpr (IS_SET(QUERY_FIELDS_MASK, 4))
        if (lhs.get_name() != rhs.get_name())
            return false;

    if constexpr (IS_SET(QUERY_FIELDS_MASK, 5))
        if (lhs.get_color() != rhs.get_color())
            return false;

    if constexpr (IS_SET(QUERY_FIELDS_MASK, 6))
        if (lhs.get_total() != rhs.get_total())
            return false;

    if constexpr (IS_SET(QUERY_FIELDS_MASK, 7))
        if (lhs.get_max_hosts() != rhs.get_max_hosts())
            return false;

    return true;
}

template <bool BOXED>
size_t consume(const SeriesMeta_BASE<BOXED>& value) noexcept
{
    size_t result = 0;
    result += consume(value.get_fields_mask());
    result += consume(value.get_time_shift());
    result += consume(value.get_tags());
    if (value.get_what()) result += consume(*value.get_what());
    if (value.get_name()) result += consume(*value.get_name());
    if (value.get_color()) result += consume(*value.get_color());
    if (value.get_total()) result += consume(*value.get_total());
    if (value.get_max_hosts()) result += consume(*value.get_max_hosts());
    return result;
}

}    // namespace opus::proxy_naive::statshouse
