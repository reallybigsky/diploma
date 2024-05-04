#pragma once

#include "baseline/Includes.hpp"
#include "baseline/statshouse/schema/Centroid.hpp"



namespace baseline::statshouse {

template <Nat...>
class MultiValue;

template <>
class MultiValue<> {
public:
    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
    static constexpr bool CTS = false;

    MultiValue() noexcept = default;

    MultiValue(const MultiValue&) noexcept = default;
    MultiValue(MultiValue&&) noexcept = default;
    MultiValue& operator=(const MultiValue&) noexcept = default;
    MultiValue& operator=(MultiValue&&) noexcept = default;

    ~MultiValue() noexcept = default;

    MultiValue(Double counter,
               Int counter_eq_1,
               Int value_set,
               Double value_min,
               Double value_max,
               Double value_sum,
               Double value_sum_square,
               const string& uniques,
               const array<Centroid>& centroids,
               Int max_host_tag,
               Int min_host_tag,
               Int max_counter_host_tag) noexcept
        : m_counter(counter)
        , m_counter_eq_1(counter_eq_1)
        , m_value_set(value_set)
        , m_value_min(value_min)
        , m_value_max(value_max)
        , m_value_sum(value_sum)
        , m_value_sum_square(value_sum_square)
        , m_uniques(uniques)
        , m_centroids(centroids)
        , m_max_host_tag(max_host_tag)
        , m_min_host_tag(min_host_tag)
        , m_max_counter_host_tag(max_counter_host_tag)
    {}

    Double get_counter() const noexcept
    {
        return m_counter;
    }

    Int get_counter_eq_1() const noexcept
    {
        return m_counter_eq_1;
    }

    Int get_value_set() const noexcept
    {
        return m_value_set;
    }

    Double get_value_min() const noexcept
    {
        return m_value_min;
    }

    Double get_value_max() const noexcept
    {
        return m_value_max;
    }

    Double get_value_sum() const noexcept
    {
        return m_value_sum;
    }

    Double get_value_sum_square() const noexcept
    {
        return m_value_sum_square;
    }

    const string& get_uniques() const noexcept
    {
        return m_uniques;
    }

    const array<Centroid>& get_centroids() const noexcept
    {
        return m_centroids;
    }

    Int get_max_host_tag() const noexcept
    {
        return m_max_host_tag;
    }

    Int get_min_host_tag() const noexcept
    {
        return m_min_host_tag;
    }

    Int get_max_counter_host_tag() const noexcept
    {
        return m_max_counter_host_tag;
    }

    static MultiValue fetch(Nat fields_mask, InputStream& stream)
    {
        Double counter;
        if (IS_SET(fields_mask, 0)) counter = Double::fetch(stream);
        Int counter_eq_1;
        if (IS_SET(fields_mask, 1)) counter_eq_1 = Int::fetch(stream);
        Int value_set;
        if (IS_SET(fields_mask, 2)) value_set = Int::fetch(stream);
        Double value_min;
        if (IS_SET(fields_mask, 3)) value_min = Double::fetch(stream);
        Double value_max;
        if (IS_SET(fields_mask, 4)) value_max = Double::fetch(stream);
        Double value_sum;
        if (IS_SET(fields_mask, 4)) value_sum = Double::fetch(stream);
        Double value_sum_square;
        if (IS_SET(fields_mask, 4)) value_sum_square = Double::fetch(stream);
        string uniques;
        if (IS_SET(fields_mask, 5)) uniques = string::fetch(stream);
        array<Centroid> centroids;
        if (IS_SET(fields_mask, 6)) centroids = array<Centroid>::fetch(stream);
        Int max_host_tag;
        if (IS_SET(fields_mask, 7)) max_host_tag = Int::fetch(stream);
        Int min_host_tag;
        if (IS_SET(fields_mask, 8)) min_host_tag = Int::fetch(stream);
        Int max_counter_host_tag;
        if (IS_SET(fields_mask, 9)) max_counter_host_tag = Int::fetch(stream);
        return {counter,
                counter_eq_1,
                value_set,
                value_min,
                value_max,
                value_sum,
                value_sum_square, uniques,
                centroids,
                max_host_tag,
                min_host_tag,
                max_counter_host_tag};
    }

    void store(Nat fields_mask, OutputStream& stream) const
    {
        if (IS_SET(fields_mask, 0)) m_counter.store(stream);
        if (IS_SET(fields_mask, 1)) m_counter_eq_1.store(stream);
        if (IS_SET(fields_mask, 2)) m_value_set.store(stream);
        if (IS_SET(fields_mask, 3)) m_value_min.store(stream);
        if (IS_SET(fields_mask, 4)) m_value_max.store(stream);
        if (IS_SET(fields_mask, 4)) m_value_sum.store(stream);
        if (IS_SET(fields_mask, 4)) m_value_sum_square.store(stream);
        if (IS_SET(fields_mask, 5)) m_uniques.store(stream);
        if (IS_SET(fields_mask, 6)) m_centroids.store(stream);
        if (IS_SET(fields_mask, 7)) m_max_host_tag.store(stream);
        if (IS_SET(fields_mask, 8)) m_min_host_tag.store(stream);
        if (IS_SET(fields_mask, 9)) m_max_counter_host_tag.store(stream);
    }

    class Builder {
    public:
        Builder& set_counter(Double::Builder value) noexcept
        {
            b_counter = value;
            return *this;
        }

        Builder& set_counter_eq_1(Int::Builder value) noexcept
        {
            b_counter_eq_1 = value;
            return *this;
        }

        Builder& set_value_set(Int::Builder value) noexcept
        {
            b_value_set = value;
            return *this;
        }

        Builder& set_value_min(Double::Builder value) noexcept
        {
            b_value_min = value;
            return *this;
        }

        Builder& set_value_max(Double::Builder value) noexcept
        {
            b_value_max = value;
            return *this;
        }

        Builder& set_value_sum(Double::Builder value) noexcept
        {
            b_value_sum = value;
            return *this;
        }

        Builder& set_value_sum_square(Double::Builder value) noexcept
        {
            b_value_sum_square = value;
            return *this;
        }

        Builder& set_uniques(const string::Builder& value) noexcept
        {
            b_uniques = value;
            return *this;
        }

        Builder& set_centroids(const array<Centroid>::Builder& value) noexcept
        {
            b_centroids = value;
            return *this;
        }

        Builder& set_max_host_tag(Int::Builder value) noexcept
        {
            b_max_host_tag = value;
            return *this;
        }

        Builder& set_min_host_tag(Int::Builder value) noexcept
        {
            b_min_host_tag = value;
            return *this;
        }

        Builder& set_max_counter_host_tag(Int::Builder value) noexcept
        {
            b_max_counter_host_tag = value;
            return *this;
        }

        void store(Nat::Builder fields_mask, OutputStream& stream) const
        {
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
        Int::Builder b_counter_eq_1;
        Int::Builder b_value_set;
        Double::Builder b_value_min;
        Double::Builder b_value_max;
        Double::Builder b_value_sum;
        Double::Builder b_value_sum_square;
        string::Builder b_uniques;
        array<Centroid>::Builder b_centroids;
        Int::Builder b_max_host_tag;
        Int::Builder b_min_host_tag;
        Int::Builder b_max_counter_host_tag;
    };

private:
    Double m_counter;
    Int m_counter_eq_1;
    Int m_value_set;
    Double m_value_min;
    Double m_value_max;
    Double m_value_sum;
    Double m_value_sum_square;
    string m_uniques;
    array<Centroid> m_centroids;
    Int m_max_host_tag;
    Int m_min_host_tag;
    Int m_max_counter_host_tag;
};

template <Nat m_FIELDS_MASK>
class MultiValue<m_FIELDS_MASK>
    : private MAYBE<Double, IS_SET(m_FIELDS_MASK, 0), 0>
    , private MAYBE<Int, IS_SET(m_FIELDS_MASK, 1), 1>
    , private MAYBE<Int, IS_SET(m_FIELDS_MASK, 2), 2>
    , private MAYBE<Double, IS_SET(m_FIELDS_MASK, 3), 3>
    , private MAYBE<Double, IS_SET(m_FIELDS_MASK, 4), 4>
    , private MAYBE<Double, IS_SET(m_FIELDS_MASK, 4), 5>
    , private MAYBE<Double, IS_SET(m_FIELDS_MASK, 4), 6>
    , private MAYBE<string, IS_SET(m_FIELDS_MASK, 5), 7>
    , private MAYBE<array<Centroid>, IS_SET(m_FIELDS_MASK, 6), 8>
    , private MAYBE<Int, IS_SET(m_FIELDS_MASK, 7), 9>
    , private MAYBE<Int, IS_SET(m_FIELDS_MASK, 8), 10>
    , private MAYBE<Int, IS_SET(m_FIELDS_MASK, 9), 11> {
private:
    using t_counter = MAYBE<Double, IS_SET(m_FIELDS_MASK, 0), 0>;
    using t_counter_eq_1 = MAYBE<Int, IS_SET(m_FIELDS_MASK, 1), 1>;
    using t_value_set = MAYBE<Int, IS_SET(m_FIELDS_MASK, 2), 2>;
    using t_value_min = MAYBE<Double, IS_SET(m_FIELDS_MASK, 3), 3>;
    using t_value_max = MAYBE<Double, IS_SET(m_FIELDS_MASK, 4), 4>;
    using t_value_sum = MAYBE<Double, IS_SET(m_FIELDS_MASK, 4), 5>;
    using t_value_sum_square = MAYBE<Double, IS_SET(m_FIELDS_MASK, 4), 6>;
    using t_uniques = MAYBE<string, IS_SET(m_FIELDS_MASK, 5), 7>;
    using t_centroids = MAYBE<array<Centroid>, IS_SET(m_FIELDS_MASK, 6), 8>;
    using t_max_host_tag = MAYBE<Int, IS_SET(m_FIELDS_MASK, 7), 9>;
    using t_min_host_tag = MAYBE<Int, IS_SET(m_FIELDS_MASK, 8), 10>;
    using t_max_counter_host_tag = MAYBE<Int, IS_SET(m_FIELDS_MASK, 9), 11>;

public:
    static constexpr Nat FIELDS_MASK = m_FIELDS_MASK;
    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
    static constexpr bool CTS = true && !IS_SET(m_FIELDS_MASK, 5) && !IS_SET(m_FIELDS_MASK, 6);

    MultiValue() noexcept = default;

    MultiValue(const MultiValue&) noexcept = default;
    MultiValue(MultiValue&&) noexcept = default;
    MultiValue& operator=(const MultiValue&) noexcept = default;
    MultiValue& operator=(MultiValue&&) noexcept = default;

    ~MultiValue() noexcept = default;

    MultiValue(Double counter,
               Int counter_eq_1,
               Int value_set,
               Double value_min,
               Double value_max,
               Double value_sum,
               Double value_sum_square,
               const string& uniques,
               const array<Centroid>& centroids,
               Int max_host_tag,
               Int min_host_tag,
               Int max_counter_host_tag) noexcept
        : t_counter(counter)
        , t_counter_eq_1(counter_eq_1)
        , t_value_set(value_set)
        , t_value_min(value_min)
        , t_value_max(value_max)
        , t_value_sum(value_sum)
        , t_value_sum_square(value_sum_square)
        , t_uniques(uniques)
        , t_centroids(centroids)
        , t_max_host_tag(max_host_tag)
        , t_min_host_tag(min_host_tag)
        , t_max_counter_host_tag(max_counter_host_tag)
    {}

    Double get_counter() const noexcept
    requires(IS_SET(m_FIELDS_MASK, 0))
    {
        return t_counter::value;
    }

    Int get_counter_eq_1() const noexcept
    requires(IS_SET(m_FIELDS_MASK, 1))
    {
        return t_counter_eq_1::value;
    }

    Int get_value_set() const noexcept
    requires(IS_SET(m_FIELDS_MASK, 2))
    {
        return t_value_set::value;
    }

    Double get_value_min() const noexcept
    requires(IS_SET(m_FIELDS_MASK, 3))
    {
        return t_value_min::value;
    }

    Double get_value_max() const noexcept
    requires(IS_SET(m_FIELDS_MASK, 4))
    {
        return t_value_max::value;
    }

    Double get_value_sum() const noexcept
    requires(IS_SET(m_FIELDS_MASK, 4))
    {
        return t_value_sum::value;
    }

    Double get_value_sum_square() const noexcept
    requires(IS_SET(m_FIELDS_MASK, 4))
    {
        return t_value_sum_square::value;
    }

    const string& get_uniques() const noexcept
    requires(IS_SET(m_FIELDS_MASK, 5))
    {
        return t_uniques::value;
    }

    const array<Centroid>& get_centroids() const noexcept
    requires(IS_SET(m_FIELDS_MASK, 6))
    {
        return t_centroids::value;
    }

    Int get_max_host_tag() const noexcept
    requires(IS_SET(m_FIELDS_MASK, 7))
    {
        return t_max_host_tag::value;
    }

    Int get_min_host_tag() const noexcept
    requires(IS_SET(m_FIELDS_MASK, 8))
    {
        return t_min_host_tag::value;
    }

    Int get_max_counter_host_tag() const noexcept
    requires(IS_SET(m_FIELDS_MASK, 9))
    {
        return t_max_counter_host_tag::value;
    }

    static MultiValue fetch(InputStream& stream)
    {
        Double counter;
        if constexpr (IS_SET(m_FIELDS_MASK, 0)) counter = Double::fetch(stream);
        Int counter_eq_1;
        if constexpr (IS_SET(m_FIELDS_MASK, 1)) counter_eq_1 = Int::fetch(stream);
        Int value_set;
        if constexpr (IS_SET(m_FIELDS_MASK, 2)) value_set = Int::fetch(stream);
        Double value_min;
        if constexpr (IS_SET(m_FIELDS_MASK, 3)) value_min = Double::fetch(stream);
        Double value_max;
        if constexpr (IS_SET(m_FIELDS_MASK, 4)) value_max = Double::fetch(stream);
        Double value_sum;
        if constexpr (IS_SET(m_FIELDS_MASK, 4)) value_sum = Double::fetch(stream);
        Double value_sum_square;
        if constexpr (IS_SET(m_FIELDS_MASK, 4)) value_sum_square = Double::fetch(stream);
        string uniques;
        if constexpr (IS_SET(m_FIELDS_MASK, 5)) uniques = string::fetch(stream);
        array<Centroid> centroids;
        if constexpr (IS_SET(m_FIELDS_MASK, 6)) centroids = array<Centroid>::fetch(stream);
        Int max_host_tag;
        if constexpr (IS_SET(m_FIELDS_MASK, 7)) max_host_tag = Int::fetch(stream);
        Int min_host_tag;
        if constexpr (IS_SET(m_FIELDS_MASK, 8)) min_host_tag = Int::fetch(stream);
        Int max_counter_host_tag;
        if constexpr (IS_SET(m_FIELDS_MASK, 9)) max_counter_host_tag = Int::fetch(stream);
        return {counter,
                counter_eq_1,
                value_set,
                value_min,
                value_max,
                value_sum,
                value_sum_square, uniques,
                centroids,
                max_host_tag,
                min_host_tag,
                max_counter_host_tag};
    }

    void store(OutputStream& stream) const
    {
        if constexpr (IS_SET(m_FIELDS_MASK, 0)) t_counter::value.store(stream);
        if constexpr (IS_SET(m_FIELDS_MASK, 1)) t_counter_eq_1::value.store(stream);
        if constexpr (IS_SET(m_FIELDS_MASK, 2)) t_value_set::value.store(stream);
        if constexpr (IS_SET(m_FIELDS_MASK, 3)) t_value_min::value.store(stream);
        if constexpr (IS_SET(m_FIELDS_MASK, 4)) t_value_max::value.store(stream);
        if constexpr (IS_SET(m_FIELDS_MASK, 4)) t_value_sum::value.store(stream);
        if constexpr (IS_SET(m_FIELDS_MASK, 4)) t_value_sum_square::value.store(stream);
        if constexpr (IS_SET(m_FIELDS_MASK, 5)) t_uniques::value.store(stream);
        if constexpr (IS_SET(m_FIELDS_MASK, 6)) t_centroids::value.store(stream);
        if constexpr (IS_SET(m_FIELDS_MASK, 7)) t_max_host_tag::value.store(stream);
        if constexpr (IS_SET(m_FIELDS_MASK, 8)) t_min_host_tag::value.store(stream);
        if constexpr (IS_SET(m_FIELDS_MASK, 9)) t_max_counter_host_tag::value.store(stream);
    }

    class Builder
        : private MAYBE<Double::Builder, IS_SET(m_FIELDS_MASK, 0), 0>
        , private MAYBE<Int::Builder, IS_SET(m_FIELDS_MASK, 1), 1>
        , private MAYBE<Int::Builder, IS_SET(m_FIELDS_MASK, 2), 2>
        , private MAYBE<Double::Builder, IS_SET(m_FIELDS_MASK, 3), 3>
        , private MAYBE<Double::Builder, IS_SET(m_FIELDS_MASK, 4), 4>
        , private MAYBE<Double::Builder, IS_SET(m_FIELDS_MASK, 4), 5>
        , private MAYBE<Double::Builder, IS_SET(m_FIELDS_MASK, 4), 6>
        , private MAYBE<string::Builder, IS_SET(m_FIELDS_MASK, 5), 7>
        , private MAYBE<array<Centroid>::Builder, IS_SET(m_FIELDS_MASK, 6), 8>
        , private MAYBE<Int::Builder, IS_SET(m_FIELDS_MASK, 7), 9>
        , private MAYBE<Int::Builder, IS_SET(m_FIELDS_MASK, 8), 10>
        , private MAYBE<Int::Builder, IS_SET(m_FIELDS_MASK, 9), 11> {
    private:
        using b_counter = MAYBE<Double::Builder, IS_SET(m_FIELDS_MASK, 0), 0>;
        using b_counter_eq_1 = MAYBE<Int::Builder, IS_SET(m_FIELDS_MASK, 1), 1>;
        using b_value_set = MAYBE<Int::Builder, IS_SET(m_FIELDS_MASK, 2), 2>;
        using b_value_min = MAYBE<Double::Builder, IS_SET(m_FIELDS_MASK, 3), 3>;
        using b_value_max = MAYBE<Double::Builder, IS_SET(m_FIELDS_MASK, 4), 4>;
        using b_value_sum = MAYBE<Double::Builder, IS_SET(m_FIELDS_MASK, 4), 5>;
        using b_value_sum_square = MAYBE<Double::Builder, IS_SET(m_FIELDS_MASK, 4), 6>;
        using b_uniques = MAYBE<string::Builder, IS_SET(m_FIELDS_MASK, 5), 7>;
        using b_centroids = MAYBE<array<Centroid>::Builder, IS_SET(m_FIELDS_MASK, 6), 8>;
        using b_max_host_tag = MAYBE<Int::Builder, IS_SET(m_FIELDS_MASK, 7), 9>;
        using b_min_host_tag = MAYBE<Int::Builder, IS_SET(m_FIELDS_MASK, 8), 10>;
        using b_max_counter_host_tag = MAYBE<Int::Builder, IS_SET(m_FIELDS_MASK, 9), 11>;

    public:
        Builder& set_counter(Double::Builder value) noexcept
        requires(IS_SET(m_FIELDS_MASK, 0))
        {
            b_counter::value = value;
            return *this;
        }

        Builder& set_counter_eq_1(Int::Builder value) noexcept
        requires(IS_SET(m_FIELDS_MASK, 1))
        {
            b_counter_eq_1::value = value;
            return *this;
        }

        Builder& set_value_set(Int::Builder value) noexcept
        requires(IS_SET(m_FIELDS_MASK, 2))
        {
            b_value_set::value = value;
            return *this;
        }

        Builder& set_value_min(Double::Builder value) noexcept
        requires(IS_SET(m_FIELDS_MASK, 3))
        {
            b_value_min::value = value;
            return *this;
        }

        Builder& set_value_max(Double::Builder value) noexcept
        requires(IS_SET(m_FIELDS_MASK, 4))
        {
            b_value_max::value = value;
            return *this;
        }

        Builder& set_value_sum(Double::Builder value) noexcept
        requires(IS_SET(m_FIELDS_MASK, 4))
        {
            b_value_sum::value = value;
            return *this;
        }

        Builder& set_value_sum_square(Double::Builder value) noexcept
        requires(IS_SET(m_FIELDS_MASK, 4))
        {
            b_value_sum_square::value = value;
            return *this;
        }

        Builder& set_uniques(const string::Builder& value) noexcept
        requires(IS_SET(m_FIELDS_MASK, 5))
        {
            b_uniques::value = value;
            return *this;
        }

        Builder& set_centroids(const array<Centroid>::Builder& value) noexcept
        requires(IS_SET(m_FIELDS_MASK, 6))
        {
            b_centroids::value = value;
            return *this;
        }

        Builder& set_max_host_tag(Int::Builder value) noexcept
        requires(IS_SET(m_FIELDS_MASK, 7))
        {
            b_max_host_tag::value = value;
            return *this;
        }

        Builder& set_min_host_tag(Int::Builder value) noexcept
        requires(IS_SET(m_FIELDS_MASK, 8))
        {
            b_min_host_tag::value = value;
            return *this;
        }

        Builder& set_max_counter_host_tag(Int::Builder value) noexcept
        requires(IS_SET(m_FIELDS_MASK, 9))
        {
            b_max_counter_host_tag::value = value;
            return *this;
        }

        bool verify() const noexcept
        {
            return (!IS_SET(m_FIELDS_MASK, 0) || b_counter::value.verify())
                   && (!IS_SET(m_FIELDS_MASK, 1) || b_counter_eq_1::value.verify())
                   && (!IS_SET(m_FIELDS_MASK, 2) || b_value_set::value.verify())
                   && (!IS_SET(m_FIELDS_MASK, 3) || b_value_min::value.verify())
                   && (!IS_SET(m_FIELDS_MASK, 4) || b_value_max::value.verify())
                   && (!IS_SET(m_FIELDS_MASK, 4) || b_value_sum::value.verify())
                   && (!IS_SET(m_FIELDS_MASK, 4) || b_value_sum_square::value.verify())
                   && (!IS_SET(m_FIELDS_MASK, 5) || b_uniques::value.verify())
                   && (!IS_SET(m_FIELDS_MASK, 6) || b_centroids::value.verify())
                   && (!IS_SET(m_FIELDS_MASK, 7) || b_max_host_tag::value.verify())
                   && (!IS_SET(m_FIELDS_MASK, 8) || b_min_host_tag::value.verify())
                   && (!IS_SET(m_FIELDS_MASK, 9) || b_max_counter_host_tag::value.verify());
        }

        void store(OutputStream& stream) const
        {
            if constexpr (IS_SET(m_FIELDS_MASK, 0)) b_counter::value.store(stream);
            if constexpr (IS_SET(m_FIELDS_MASK, 1)) b_counter_eq_1::value.store(stream);
            if constexpr (IS_SET(m_FIELDS_MASK, 2)) b_value_set::value.store(stream);
            if constexpr (IS_SET(m_FIELDS_MASK, 3)) b_value_min::value.store(stream);
            if constexpr (IS_SET(m_FIELDS_MASK, 4)) b_value_max::value.store(stream);
            if constexpr (IS_SET(m_FIELDS_MASK, 4)) b_value_sum::value.store(stream);
            if constexpr (IS_SET(m_FIELDS_MASK, 4)) b_value_sum_square::value.store(stream);
            if constexpr (IS_SET(m_FIELDS_MASK, 5)) b_uniques::value.store(stream);
            if constexpr (IS_SET(m_FIELDS_MASK, 6)) b_centroids::value.store(stream);
            if constexpr (IS_SET(m_FIELDS_MASK, 7)) b_max_host_tag::value.store(stream);
            if constexpr (IS_SET(m_FIELDS_MASK, 8)) b_min_host_tag::value.store(stream);
            if constexpr (IS_SET(m_FIELDS_MASK, 9)) b_max_counter_host_tag::value.store(stream);
        }

    private:
    };

private:
};

}    // namespace baseline::statshouse
