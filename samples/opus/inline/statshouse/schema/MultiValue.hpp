#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/inline/statshouse/schema/Centroid.hpp"



namespace opus::inl::statshouse {

template <bool BOXED, Nat...>
class MultiValue_BASE;

template <bool BOXED>
class MultiValue_BASE<BOXED> {
public:
    static constexpr Magic MAGIC = 1256332;
    static constexpr bool STATIC = false;

    MultiValue_BASE() noexcept = default;

    MultiValue_BASE(const MultiValue_BASE&) noexcept = default;
    MultiValue_BASE(MultiValue_BASE&&) noexcept = default;
    MultiValue_BASE& operator=(const MultiValue_BASE&) noexcept = default;
    MultiValue_BASE& operator=(MultiValue_BASE&&) noexcept = default;

    ~MultiValue_BASE() noexcept = default;

    const std::optional<Double>& get_counter() const noexcept
    {
        return m_counter;
    }

    const std::optional<true_t>& get_counter_eq_1() const noexcept
    {
        return m_counter_eq_1;
    }

    const std::optional<true_t>& get_value_set() const noexcept
    {
        return m_value_set;
    }

    const std::optional<Double>& get_value_min() const noexcept
    {
        return m_value_min;
    }

    const std::optional<Double>& get_value_max() const noexcept
    {
        return m_value_max;
    }

    const std::optional<Double>& get_value_sum() const noexcept
    {
        return m_value_sum;
    }

    const std::optional<Double>& get_value_sum_square() const noexcept
    {
        return m_value_sum_square;
    }

    const std::optional<string>& get_uniques() const noexcept
    {
        return m_uniques;
    }

    const std::optional<array<centroid>>& get_centroids() const noexcept
    {
        return m_centroids;
    }

    const std::optional<Int>& get_max_host_tag() const noexcept
    {
        return m_max_host_tag;
    }

    const std::optional<Int>& get_min_host_tag() const noexcept
    {
        return m_min_host_tag;
    }

    const std::optional<Int>& get_max_counter_host_tag() const noexcept
    {
        return m_max_counter_host_tag;
    }

    static MultiValue_BASE fetch(InputStream& stream, Nat fields_mask)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        std::optional<Double> counter;
        if (IS_SET(fields_mask, 0)) counter = Double::fetch(stream);
        std::optional<true_t> counter_eq_1;
        if (IS_SET(fields_mask, 1)) counter_eq_1 = true_t::fetch(stream);
        std::optional<true_t> value_set;
        if (IS_SET(fields_mask, 2)) value_set = true_t::fetch(stream);
        std::optional<Double> value_min;
        if (IS_SET(fields_mask, 3)) value_min = Double::fetch(stream);
        std::optional<Double> value_max;
        if (IS_SET(fields_mask, 4)) value_max = Double::fetch(stream);
        std::optional<Double> value_sum;
        if (IS_SET(fields_mask, 4)) value_sum = Double::fetch(stream);
        std::optional<Double> value_sum_square;
        if (IS_SET(fields_mask, 4)) value_sum_square = Double::fetch(stream);
        std::optional<string> uniques;
        if (IS_SET(fields_mask, 5)) uniques = string::fetch(stream);
        std::optional<array<centroid>> centroids;
        if (IS_SET(fields_mask, 6)) centroids = array<centroid>::fetch(stream);
        std::optional<Int> max_host_tag;
        if (IS_SET(fields_mask, 7)) max_host_tag = Int::fetch(stream);
        std::optional<Int> min_host_tag;
        if (IS_SET(fields_mask, 8)) min_host_tag = Int::fetch(stream);
        std::optional<Int> max_counter_host_tag;
        if (IS_SET(fields_mask, 9)) max_counter_host_tag = Int::fetch(stream);
        MultiValue_BASE result(std::move(counter),
                               std::move(counter_eq_1),
                               std::move(value_set),
                               std::move(value_min),
                               std::move(value_max),
                               std::move(value_sum),
                               std::move(value_sum_square),
                               std::move(uniques),
                               std::move(centroids),
                               std::move(max_host_tag),
                               std::move(min_host_tag),
                               std::move(max_counter_host_tag));
        return result;
    }

    void store(OutputStream& stream, Nat fields_mask) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        if (IS_SET(fields_mask, 0)) m_counter->store(stream);
        if (IS_SET(fields_mask, 1)) m_counter_eq_1->store(stream);
        if (IS_SET(fields_mask, 2)) m_value_set->store(stream);
        if (IS_SET(fields_mask, 3)) m_value_min->store(stream);
        if (IS_SET(fields_mask, 4)) m_value_max->store(stream);
        if (IS_SET(fields_mask, 4)) m_value_sum->store(stream);
        if (IS_SET(fields_mask, 4)) m_value_sum_square->store(stream);
        if (IS_SET(fields_mask, 5)) m_uniques->store(stream);
        if (IS_SET(fields_mask, 6)) m_centroids->store(stream);
        if (IS_SET(fields_mask, 7)) m_max_host_tag->store(stream);
        if (IS_SET(fields_mask, 8)) m_min_host_tag->store(stream);
        if (IS_SET(fields_mask, 9)) m_max_counter_host_tag->store(stream);
    }

    class Builder {
    public:
        using TYPE = MultiValue_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const MultiValue_BASE<RHS_BOXED>& rhs) noexcept
        {
            return (!rhs.get_counter() || lhs.b_counter == rhs.get_counter())
                   && (!rhs.get_counter_eq_1() || lhs.b_counter_eq_1 == rhs.get_counter_eq_1())
                   && (!rhs.get_value_set() || lhs.b_value_set == rhs.get_value_set())
                   && (!rhs.get_value_min() || lhs.b_value_min == rhs.get_value_min())
                   && (!rhs.get_value_max() || lhs.b_value_max == rhs.get_value_max())
                   && (!rhs.get_value_sum() || lhs.b_value_sum == rhs.get_value_sum())
                   && (!rhs.get_value_sum_square() || lhs.b_value_sum_square == rhs.get_value_sum_square())
                   && (!rhs.get_uniques() || lhs.b_uniques == rhs.get_uniques())
                   && (!rhs.get_centroids() || lhs.b_centroids == rhs.get_centroids())
                   && (!rhs.get_max_host_tag() || lhs.b_max_host_tag == rhs.get_max_host_tag())
                   && (!rhs.get_min_host_tag() || lhs.b_min_host_tag == rhs.get_min_host_tag())
                   && (!rhs.get_max_counter_host_tag() || lhs.b_max_counter_host_tag == rhs.get_max_counter_host_tag());
        }

        template <size_t SIZE_1, size_t SIZE_2>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_counter(Double::Builder::random(engine))
                    .set_counter_eq_1(true_t::Builder::random(engine))
                    .set_value_set(true_t::Builder::random(engine))
                    .set_value_min(Double::Builder::random(engine))
                    .set_value_max(Double::Builder::random(engine))
                    .set_value_sum(Double::Builder::random(engine))
                    .set_value_sum_square(Double::Builder::random(engine))
                    .set_uniques(string::Builder::random<SIZE_1>(engine))
                    .set_centroids(array<centroid>::Builder::random<SIZE_2>(engine))
                    .set_max_host_tag(Int::Builder::random(engine))
                    .set_min_host_tag(Int::Builder::random(engine))
                    .set_max_counter_host_tag(Int::Builder::random(engine));
        }

        Builder& set_counter(const Double::Builder& value) noexcept
        {
            b_counter = value;
            return *this;
        }

        Builder& set_counter_eq_1(const true_t::Builder& value) noexcept
        {
            b_counter_eq_1 = value;
            return *this;
        }

        Builder& set_value_set(const true_t::Builder& value) noexcept
        {
            b_value_set = value;
            return *this;
        }

        Builder& set_value_min(const Double::Builder& value) noexcept
        {
            b_value_min = value;
            return *this;
        }

        Builder& set_value_max(const Double::Builder& value) noexcept
        {
            b_value_max = value;
            return *this;
        }

        Builder& set_value_sum(const Double::Builder& value) noexcept
        {
            b_value_sum = value;
            return *this;
        }

        Builder& set_value_sum_square(const Double::Builder& value) noexcept
        {
            b_value_sum_square = value;
            return *this;
        }

        Builder& set_uniques(const string::Builder& value) noexcept
        {
            b_uniques = value;
            return *this;
        }

        Builder& set_centroids(const array<centroid>::Builder& value) noexcept
        {
            b_centroids = value;
            return *this;
        }

        Builder& set_max_host_tag(const Int::Builder& value) noexcept
        {
            b_max_host_tag = value;
            return *this;
        }

        Builder& set_min_host_tag(const Int::Builder& value) noexcept
        {
            b_min_host_tag = value;
            return *this;
        }

        Builder& set_max_counter_host_tag(const Int::Builder& value) noexcept
        {
            b_max_counter_host_tag = value;
            return *this;
        }

        void store(OutputStream& stream, Nat fields_mask) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            if (IS_SET(fields_mask, 0)) b_counter.store(stream);
            if (IS_SET(fields_mask, 1)) b_counter_eq_1.store(stream);
            if (IS_SET(fields_mask, 2)) b_value_set.store(stream);
            if (IS_SET(fields_mask, 3)) b_value_min.store(stream);
            if (IS_SET(fields_mask, 4)) b_value_max.store(stream);
            if (IS_SET(fields_mask, 4)) b_value_sum.store(stream);
            if (IS_SET(fields_mask, 4)) b_value_sum_square.store(stream);
            if (IS_SET(fields_mask, 5)) b_uniques.store(stream);
            if (IS_SET(fields_mask, 6)) b_centroids.store(stream);
            if (IS_SET(fields_mask, 7)) b_max_host_tag.store(stream);
            if (IS_SET(fields_mask, 8)) b_min_host_tag.store(stream);
            if (IS_SET(fields_mask, 9)) b_max_counter_host_tag.store(stream);
        }

    private:
        Double::Builder b_counter;
        true_t::Builder b_counter_eq_1;
        true_t::Builder b_value_set;
        Double::Builder b_value_min;
        Double::Builder b_value_max;
        Double::Builder b_value_sum;
        Double::Builder b_value_sum_square;
        string::Builder b_uniques;
        array<centroid>::Builder b_centroids;
        Int::Builder b_max_host_tag;
        Int::Builder b_min_host_tag;
        Int::Builder b_max_counter_host_tag;
    };

private:
    MultiValue_BASE(std::optional<Double>&& counter,
                    std::optional<true_t>&& counter_eq_1,
                    std::optional<true_t>&& value_set,
                    std::optional<Double>&& value_min,
                    std::optional<Double>&& value_max,
                    std::optional<Double>&& value_sum,
                    std::optional<Double>&& value_sum_square,
                    std::optional<string>&& uniques,
                    std::optional<array<centroid>>&& centroids,
                    std::optional<Int>&& max_host_tag,
                    std::optional<Int>&& min_host_tag,
                    std::optional<Int>&& max_counter_host_tag) noexcept
        : m_counter(std::move(counter))
        , m_counter_eq_1(std::move(counter_eq_1))
        , m_value_set(std::move(value_set))
        , m_value_min(std::move(value_min))
        , m_value_max(std::move(value_max))
        , m_value_sum(std::move(value_sum))
        , m_value_sum_square(std::move(value_sum_square))
        , m_uniques(std::move(uniques))
        , m_centroids(std::move(centroids))
        , m_max_host_tag(std::move(max_host_tag))
        , m_min_host_tag(std::move(min_host_tag))
        , m_max_counter_host_tag(std::move(max_counter_host_tag))
    {}

    std::optional<Double> m_counter;
    std::optional<true_t> m_counter_eq_1;
    std::optional<true_t> m_value_set;
    std::optional<Double> m_value_min;
    std::optional<Double> m_value_max;
    std::optional<Double> m_value_sum;
    std::optional<Double> m_value_sum_square;
    std::optional<string> m_uniques;
    std::optional<array<centroid>> m_centroids;
    std::optional<Int> m_max_host_tag;
    std::optional<Int> m_min_host_tag;
    std::optional<Int> m_max_counter_host_tag;
};

template <bool BOXED, Nat FIELDS_MASK>
class MultiValue_BASE<BOXED, FIELDS_MASK>
    : private MAYBE<Double, IS_SET(FIELDS_MASK, 0), utils::commutative(1256332, 0)>
    , private MAYBE<true_t, IS_SET(FIELDS_MASK, 1), utils::commutative(1256332, 1)>
    , private MAYBE<true_t, IS_SET(FIELDS_MASK, 2), utils::commutative(1256332, 2)>
    , private MAYBE<Double, IS_SET(FIELDS_MASK, 3), utils::commutative(1256332, 3)>
    , private MAYBE<Double, IS_SET(FIELDS_MASK, 4), utils::commutative(1256332, 4)>
    , private MAYBE<Double, IS_SET(FIELDS_MASK, 4), utils::commutative(1256332, 5)>
    , private MAYBE<Double, IS_SET(FIELDS_MASK, 4), utils::commutative(1256332, 6)>
    , private MAYBE<string, IS_SET(FIELDS_MASK, 5), utils::commutative(1256332, 7)>
    , private MAYBE<array<centroid>, IS_SET(FIELDS_MASK, 6), utils::commutative(1256332, 8)>
    , private MAYBE<Int, IS_SET(FIELDS_MASK, 7), utils::commutative(1256332, 9)>
    , private MAYBE<Int, IS_SET(FIELDS_MASK, 8), utils::commutative(1256332, 10)>
    , private MAYBE<Int, IS_SET(FIELDS_MASK, 9), utils::commutative(1256332, 11)> {
private:
    using m_counter = MAYBE<Double, IS_SET(FIELDS_MASK, 0), utils::commutative(1256332, 0)>;
    using m_counter_eq_1 = MAYBE<true_t, IS_SET(FIELDS_MASK, 1), utils::commutative(1256332, 1)>;
    using m_value_set = MAYBE<true_t, IS_SET(FIELDS_MASK, 2), utils::commutative(1256332, 2)>;
    using m_value_min = MAYBE<Double, IS_SET(FIELDS_MASK, 3), utils::commutative(1256332, 3)>;
    using m_value_max = MAYBE<Double, IS_SET(FIELDS_MASK, 4), utils::commutative(1256332, 4)>;
    using m_value_sum = MAYBE<Double, IS_SET(FIELDS_MASK, 4), utils::commutative(1256332, 5)>;
    using m_value_sum_square = MAYBE<Double, IS_SET(FIELDS_MASK, 4), utils::commutative(1256332, 6)>;
    using m_uniques = MAYBE<string, IS_SET(FIELDS_MASK, 5), utils::commutative(1256332, 7)>;
    using m_centroids = MAYBE<array<centroid>, IS_SET(FIELDS_MASK, 6), utils::commutative(1256332, 8)>;
    using m_max_host_tag = MAYBE<Int, IS_SET(FIELDS_MASK, 7), utils::commutative(1256332, 9)>;
    using m_min_host_tag = MAYBE<Int, IS_SET(FIELDS_MASK, 8), utils::commutative(1256332, 10)>;
    using m_max_counter_host_tag = MAYBE<Int, IS_SET(FIELDS_MASK, 9), utils::commutative(1256332, 11)>;

public:
    static constexpr Magic MAGIC = 1256332;
    static constexpr bool STATIC = false;

    MultiValue_BASE() noexcept = default;

    MultiValue_BASE(const MultiValue_BASE&) noexcept = default;
    MultiValue_BASE(MultiValue_BASE&&) noexcept = default;
    MultiValue_BASE& operator=(const MultiValue_BASE&) noexcept = default;
    MultiValue_BASE& operator=(MultiValue_BASE&&) noexcept = default;

    ~MultiValue_BASE() noexcept = default;

    const Double& get_counter() const noexcept
    requires(IS_SET(FIELDS_MASK, 0))
    {
        return m_counter::value;
    }

    const true_t& get_counter_eq_1() const noexcept
    requires(IS_SET(FIELDS_MASK, 1))
    {
        return m_counter_eq_1::value;
    }

    const true_t& get_value_set() const noexcept
    requires(IS_SET(FIELDS_MASK, 2))
    {
        return m_value_set::value;
    }

    const Double& get_value_min() const noexcept
    requires(IS_SET(FIELDS_MASK, 3))
    {
        return m_value_min::value;
    }

    const Double& get_value_max() const noexcept
    requires(IS_SET(FIELDS_MASK, 4))
    {
        return m_value_max::value;
    }

    const Double& get_value_sum() const noexcept
    requires(IS_SET(FIELDS_MASK, 4))
    {
        return m_value_sum::value;
    }

    const Double& get_value_sum_square() const noexcept
    requires(IS_SET(FIELDS_MASK, 4))
    {
        return m_value_sum_square::value;
    }

    const string& get_uniques() const noexcept
    requires(IS_SET(FIELDS_MASK, 5))
    {
        return m_uniques::value;
    }

    const array<centroid>& get_centroids() const noexcept
    requires(IS_SET(FIELDS_MASK, 6))
    {
        return m_centroids::value;
    }

    const Int& get_max_host_tag() const noexcept
    requires(IS_SET(FIELDS_MASK, 7))
    {
        return m_max_host_tag::value;
    }

    const Int& get_min_host_tag() const noexcept
    requires(IS_SET(FIELDS_MASK, 8))
    {
        return m_min_host_tag::value;
    }

    const Int& get_max_counter_host_tag() const noexcept
    requires(IS_SET(FIELDS_MASK, 9))
    {
        return m_max_counter_host_tag::value;
    }

    static MultiValue_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Double counter;
        if constexpr (IS_SET(FIELDS_MASK, 0)) counter = Double::fetch(stream);
        true_t counter_eq_1;
        if constexpr (IS_SET(FIELDS_MASK, 1)) counter_eq_1 = true_t::fetch(stream);
        true_t value_set;
        if constexpr (IS_SET(FIELDS_MASK, 2)) value_set = true_t::fetch(stream);
        Double value_min;
        if constexpr (IS_SET(FIELDS_MASK, 3)) value_min = Double::fetch(stream);
        Double value_max;
        if constexpr (IS_SET(FIELDS_MASK, 4)) value_max = Double::fetch(stream);
        Double value_sum;
        if constexpr (IS_SET(FIELDS_MASK, 4)) value_sum = Double::fetch(stream);
        Double value_sum_square;
        if constexpr (IS_SET(FIELDS_MASK, 4)) value_sum_square = Double::fetch(stream);
        string uniques;
        if constexpr (IS_SET(FIELDS_MASK, 5)) uniques = string::fetch(stream);
        array<centroid> centroids;
        if constexpr (IS_SET(FIELDS_MASK, 6)) centroids = array<centroid>::fetch(stream);
        Int max_host_tag;
        if constexpr (IS_SET(FIELDS_MASK, 7)) max_host_tag = Int::fetch(stream);
        Int min_host_tag;
        if constexpr (IS_SET(FIELDS_MASK, 8)) min_host_tag = Int::fetch(stream);
        Int max_counter_host_tag;
        if constexpr (IS_SET(FIELDS_MASK, 9)) max_counter_host_tag = Int::fetch(stream);
        MultiValue_BASE result(std::move(counter),
                               std::move(counter_eq_1),
                               std::move(value_set),
                               std::move(value_min),
                               std::move(value_max),
                               std::move(value_sum),
                               std::move(value_sum_square),
                               std::move(uniques),
                               std::move(centroids),
                               std::move(max_host_tag),
                               std::move(min_host_tag),
                               std::move(max_counter_host_tag));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        if constexpr (IS_SET(FIELDS_MASK, 0)) m_counter::value.store(stream);
        if constexpr (IS_SET(FIELDS_MASK, 1)) m_counter_eq_1::value.store(stream);
        if constexpr (IS_SET(FIELDS_MASK, 2)) m_value_set::value.store(stream);
        if constexpr (IS_SET(FIELDS_MASK, 3)) m_value_min::value.store(stream);
        if constexpr (IS_SET(FIELDS_MASK, 4)) m_value_max::value.store(stream);
        if constexpr (IS_SET(FIELDS_MASK, 4)) m_value_sum::value.store(stream);
        if constexpr (IS_SET(FIELDS_MASK, 4)) m_value_sum_square::value.store(stream);
        if constexpr (IS_SET(FIELDS_MASK, 5)) m_uniques::value.store(stream);
        if constexpr (IS_SET(FIELDS_MASK, 6)) m_centroids::value.store(stream);
        if constexpr (IS_SET(FIELDS_MASK, 7)) m_max_host_tag::value.store(stream);
        if constexpr (IS_SET(FIELDS_MASK, 8)) m_min_host_tag::value.store(stream);
        if constexpr (IS_SET(FIELDS_MASK, 9)) m_max_counter_host_tag::value.store(stream);
    }

    class Builder
        : private MAYBE<Double::Builder, IS_SET(FIELDS_MASK, 0), utils::commutative(1256332, 0)>
        , private MAYBE<true_t::Builder, IS_SET(FIELDS_MASK, 1), utils::commutative(1256332, 1)>
        , private MAYBE<true_t::Builder, IS_SET(FIELDS_MASK, 2), utils::commutative(1256332, 2)>
        , private MAYBE<Double::Builder, IS_SET(FIELDS_MASK, 3), utils::commutative(1256332, 3)>
        , private MAYBE<Double::Builder, IS_SET(FIELDS_MASK, 4), utils::commutative(1256332, 4)>
        , private MAYBE<Double::Builder, IS_SET(FIELDS_MASK, 4), utils::commutative(1256332, 5)>
        , private MAYBE<Double::Builder, IS_SET(FIELDS_MASK, 4), utils::commutative(1256332, 6)>
        , private MAYBE<string::Builder, IS_SET(FIELDS_MASK, 5), utils::commutative(1256332, 7)>
        , private MAYBE<array<centroid>::Builder, IS_SET(FIELDS_MASK, 6), utils::commutative(1256332, 8)>
        , private MAYBE<Int::Builder, IS_SET(FIELDS_MASK, 7), utils::commutative(1256332, 9)>
        , private MAYBE<Int::Builder, IS_SET(FIELDS_MASK, 8), utils::commutative(1256332, 10)>
        , private MAYBE<Int::Builder, IS_SET(FIELDS_MASK, 9), utils::commutative(1256332, 11)> {
    private:
        using b_counter = MAYBE<Double::Builder, IS_SET(FIELDS_MASK, 0), utils::commutative(1256332, 0)>;
        using b_counter_eq_1 = MAYBE<true_t::Builder, IS_SET(FIELDS_MASK, 1), utils::commutative(1256332, 1)>;
        using b_value_set = MAYBE<true_t::Builder, IS_SET(FIELDS_MASK, 2), utils::commutative(1256332, 2)>;
        using b_value_min = MAYBE<Double::Builder, IS_SET(FIELDS_MASK, 3), utils::commutative(1256332, 3)>;
        using b_value_max = MAYBE<Double::Builder, IS_SET(FIELDS_MASK, 4), utils::commutative(1256332, 4)>;
        using b_value_sum = MAYBE<Double::Builder, IS_SET(FIELDS_MASK, 4), utils::commutative(1256332, 5)>;
        using b_value_sum_square = MAYBE<Double::Builder, IS_SET(FIELDS_MASK, 4), utils::commutative(1256332, 6)>;
        using b_uniques = MAYBE<string::Builder, IS_SET(FIELDS_MASK, 5), utils::commutative(1256332, 7)>;
        using b_centroids = MAYBE<array<centroid>::Builder, IS_SET(FIELDS_MASK, 6), utils::commutative(1256332, 8)>;
        using b_max_host_tag = MAYBE<Int::Builder, IS_SET(FIELDS_MASK, 7), utils::commutative(1256332, 9)>;
        using b_min_host_tag = MAYBE<Int::Builder, IS_SET(FIELDS_MASK, 8), utils::commutative(1256332, 10)>;
        using b_max_counter_host_tag = MAYBE<Int::Builder, IS_SET(FIELDS_MASK, 9), utils::commutative(1256332, 11)>;

    public:
        using TYPE = MultiValue_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const MultiValue_BASE<RHS_BOXED, FIELDS_MASK>& rhs) noexcept
        {
            if constexpr (IS_SET(FIELDS_MASK, 0))
                if (lhs.b_counter::value != rhs.get_counter())
                    return false;

            if constexpr (IS_SET(FIELDS_MASK, 1))
                if (lhs.b_counter_eq_1::value != rhs.get_counter_eq_1())
                    return false;

            if constexpr (IS_SET(FIELDS_MASK, 2))
                if (lhs.b_value_set::value != rhs.get_value_set())
                    return false;

            if constexpr (IS_SET(FIELDS_MASK, 3))
                if (lhs.b_value_min::value != rhs.get_value_min())
                    return false;

            if constexpr (IS_SET(FIELDS_MASK, 4))
                if (lhs.b_value_max::value != rhs.get_value_max())
                    return false;

            if constexpr (IS_SET(FIELDS_MASK, 4))
                if (lhs.b_value_sum::value != rhs.get_value_sum())
                    return false;

            if constexpr (IS_SET(FIELDS_MASK, 4))
                if (lhs.b_value_sum_square::value != rhs.get_value_sum_square())
                    return false;

            if constexpr (IS_SET(FIELDS_MASK, 5))
                if (lhs.b_uniques::value != rhs.get_uniques())
                    return false;

            if constexpr (IS_SET(FIELDS_MASK, 6))
                if (lhs.b_centroids::value != rhs.get_centroids())
                    return false;

            if constexpr (IS_SET(FIELDS_MASK, 7))
                if (lhs.b_max_host_tag::value != rhs.get_max_host_tag())
                    return false;

            if constexpr (IS_SET(FIELDS_MASK, 8))
                if (lhs.b_min_host_tag::value != rhs.get_min_host_tag())
                    return false;

            if constexpr (IS_SET(FIELDS_MASK, 9))
                if (lhs.b_max_counter_host_tag::value != rhs.get_max_counter_host_tag())
                    return false;

            return true;
        }

        Builder& set_counter(const Double::Builder& value) noexcept
        requires(IS_SET(FIELDS_MASK, 0))
        {
            b_counter::value = value;
            return *this;
        }

        Builder& set_counter_eq_1(const true_t::Builder& value) noexcept
        requires(IS_SET(FIELDS_MASK, 1))
        {
            b_counter_eq_1::value = value;
            return *this;
        }

        Builder& set_value_set(const true_t::Builder& value) noexcept
        requires(IS_SET(FIELDS_MASK, 2))
        {
            b_value_set::value = value;
            return *this;
        }

        Builder& set_value_min(const Double::Builder& value) noexcept
        requires(IS_SET(FIELDS_MASK, 3))
        {
            b_value_min::value = value;
            return *this;
        }

        Builder& set_value_max(const Double::Builder& value) noexcept
        requires(IS_SET(FIELDS_MASK, 4))
        {
            b_value_max::value = value;
            return *this;
        }

        Builder& set_value_sum(const Double::Builder& value) noexcept
        requires(IS_SET(FIELDS_MASK, 4))
        {
            b_value_sum::value = value;
            return *this;
        }

        Builder& set_value_sum_square(const Double::Builder& value) noexcept
        requires(IS_SET(FIELDS_MASK, 4))
        {
            b_value_sum_square::value = value;
            return *this;
        }

        Builder& set_uniques(const string::Builder& value) noexcept
        requires(IS_SET(FIELDS_MASK, 5))
        {
            b_uniques::value = value;
            return *this;
        }

        Builder& set_centroids(const array<centroid>::Builder& value) noexcept
        requires(IS_SET(FIELDS_MASK, 6))
        {
            b_centroids::value = value;
            return *this;
        }

        Builder& set_max_host_tag(const Int::Builder& value) noexcept
        requires(IS_SET(FIELDS_MASK, 7))
        {
            b_max_host_tag::value = value;
            return *this;
        }

        Builder& set_min_host_tag(const Int::Builder& value) noexcept
        requires(IS_SET(FIELDS_MASK, 8))
        {
            b_min_host_tag::value = value;
            return *this;
        }

        Builder& set_max_counter_host_tag(const Int::Builder& value) noexcept
        requires(IS_SET(FIELDS_MASK, 9))
        {
            b_max_counter_host_tag::value = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            if constexpr (IS_SET(FIELDS_MASK, 0)) b_counter::value.store(stream);
            if constexpr (IS_SET(FIELDS_MASK, 1)) b_counter_eq_1::value.store(stream);
            if constexpr (IS_SET(FIELDS_MASK, 2)) b_value_set::value.store(stream);
            if constexpr (IS_SET(FIELDS_MASK, 3)) b_value_min::value.store(stream);
            if constexpr (IS_SET(FIELDS_MASK, 4)) b_value_max::value.store(stream);
            if constexpr (IS_SET(FIELDS_MASK, 4)) b_value_sum::value.store(stream);
            if constexpr (IS_SET(FIELDS_MASK, 4)) b_value_sum_square::value.store(stream);
            if constexpr (IS_SET(FIELDS_MASK, 5)) b_uniques::value.store(stream);
            if constexpr (IS_SET(FIELDS_MASK, 6)) b_centroids::value.store(stream);
            if constexpr (IS_SET(FIELDS_MASK, 7)) b_max_host_tag::value.store(stream);
            if constexpr (IS_SET(FIELDS_MASK, 8)) b_min_host_tag::value.store(stream);
            if constexpr (IS_SET(FIELDS_MASK, 9)) b_max_counter_host_tag::value.store(stream);
        }

    private:
    };

private:
    MultiValue_BASE(Double&& counter,
                    true_t&& counter_eq_1,
                    true_t&& value_set,
                    Double&& value_min,
                    Double&& value_max,
                    Double&& value_sum,
                    Double&& value_sum_square,
                    string&& uniques,
                    array<centroid>&& centroids,
                    Int&& max_host_tag,
                    Int&& min_host_tag,
                    Int&& max_counter_host_tag) noexcept
        : m_counter(std::move(counter))
        , m_counter_eq_1(std::move(counter_eq_1))
        , m_value_set(std::move(value_set))
        , m_value_min(std::move(value_min))
        , m_value_max(std::move(value_max))
        , m_value_sum(std::move(value_sum))
        , m_value_sum_square(std::move(value_sum_square))
        , m_uniques(std::move(uniques))
        , m_centroids(std::move(centroids))
        , m_max_host_tag(std::move(max_host_tag))
        , m_min_host_tag(std::move(min_host_tag))
        , m_max_counter_host_tag(std::move(max_counter_host_tag))
    {}
};

using multiValue = MultiValue_BASE<false>;
using MultiValue = MultiValue_BASE<true>;

template <Nat FIELDS_MASK>
using multiValue_s = MultiValue_BASE<false, FIELDS_MASK>;

template <Nat FIELDS_MASK>
using MultiValue_s = MultiValue_BASE<true, FIELDS_MASK>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const MultiValue_BASE<LHS_BOXED>& lhs, const MultiValue_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_counter() == rhs.get_counter()
           && lhs.get_counter_eq_1() == rhs.get_counter_eq_1()
           && lhs.get_value_set() == rhs.get_value_set()
           && lhs.get_value_min() == rhs.get_value_min()
           && lhs.get_value_max() == rhs.get_value_max()
           && lhs.get_value_sum() == rhs.get_value_sum()
           && lhs.get_value_sum_square() == rhs.get_value_sum_square()
           && lhs.get_uniques() == rhs.get_uniques()
           && lhs.get_centroids() == rhs.get_centroids()
           && lhs.get_max_host_tag() == rhs.get_max_host_tag()
           && lhs.get_min_host_tag() == rhs.get_min_host_tag()
           && lhs.get_max_counter_host_tag() == rhs.get_max_counter_host_tag();
}

template <bool LHS_BOXED, bool RHS_BOXED, Nat FIELDS_MASK>
bool operator==(const MultiValue_BASE<LHS_BOXED, FIELDS_MASK>& lhs, const MultiValue_BASE<RHS_BOXED, FIELDS_MASK>& rhs) noexcept
{
    if constexpr (IS_SET(FIELDS_MASK, 0))
        if (lhs.get_counter() != rhs.get_counter())
            return false;

    if constexpr (IS_SET(FIELDS_MASK, 1))
        if (lhs.get_counter_eq_1() != rhs.get_counter_eq_1())
            return false;

    if constexpr (IS_SET(FIELDS_MASK, 2))
        if (lhs.get_value_set() != rhs.get_value_set())
            return false;

    if constexpr (IS_SET(FIELDS_MASK, 3))
        if (lhs.get_value_min() != rhs.get_value_min())
            return false;

    if constexpr (IS_SET(FIELDS_MASK, 4))
        if (lhs.get_value_max() != rhs.get_value_max())
            return false;

    if constexpr (IS_SET(FIELDS_MASK, 4))
        if (lhs.get_value_sum() != rhs.get_value_sum())
            return false;

    if constexpr (IS_SET(FIELDS_MASK, 4))
        if (lhs.get_value_sum_square() != rhs.get_value_sum_square())
            return false;

    if constexpr (IS_SET(FIELDS_MASK, 5))
        if (lhs.get_uniques() != rhs.get_uniques())
            return false;

    if constexpr (IS_SET(FIELDS_MASK, 6))
        if (lhs.get_centroids() != rhs.get_centroids())
            return false;

    if constexpr (IS_SET(FIELDS_MASK, 7))
        if (lhs.get_max_host_tag() != rhs.get_max_host_tag())
            return false;

    if constexpr (IS_SET(FIELDS_MASK, 8))
        if (lhs.get_min_host_tag() != rhs.get_min_host_tag())
            return false;

    if constexpr (IS_SET(FIELDS_MASK, 9))
        if (lhs.get_max_counter_host_tag() != rhs.get_max_counter_host_tag())
            return false;

    return true;
}

template <bool BOXED>
size_t consume(const MultiValue_BASE<BOXED>& value) noexcept
{
    size_t result = 0;
    if (value.get_counter()) result += consume(*value.get_counter());
    if (value.get_counter_eq_1()) result += consume(*value.get_counter_eq_1());
    if (value.get_value_set()) result += consume(*value.get_value_set());
    if (value.get_value_min()) result += consume(*value.get_value_min());
    if (value.get_value_max()) result += consume(*value.get_value_max());
    if (value.get_value_sum()) result += consume(*value.get_value_sum());
    if (value.get_value_sum_square()) result += consume(*value.get_value_sum_square());
    if (value.get_uniques()) result += consume(*value.get_uniques());
    if (value.get_centroids()) result += consume(*value.get_centroids());
    if (value.get_max_host_tag()) result += consume(*value.get_max_host_tag());
    if (value.get_min_host_tag()) result += consume(*value.get_min_host_tag());
    if (value.get_max_counter_host_tag()) result += consume(*value.get_max_counter_host_tag());
    return result;
}

}    // namespace opus::inl::statshouse
