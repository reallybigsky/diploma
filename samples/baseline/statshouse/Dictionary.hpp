#pragma once

#include "Common.hpp"
#include "baseline/Array.hpp"



namespace baseline::statshouse {

template <TLType T>
class DictionaryField {
public:
    using TYPE_PARAM = T;
    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
    static constexpr bool CTS = false;

    DictionaryField() noexcept = default;

    DictionaryField(const DictionaryField&) noexcept = default;
    DictionaryField(DictionaryField&&) noexcept = default;
    DictionaryField& operator=(const DictionaryField&) noexcept = default;
    DictionaryField& operator=(DictionaryField&&) noexcept = default;

    ~DictionaryField() noexcept = default;

    DictionaryField(const string& key,
                    const T& value) noexcept
        : m_key(key)
        , m_value(value)
    {}

    const string& get_key() const noexcept
    {
        return m_key;
    }

    const T& get_value() const noexcept
    {
        return m_value;
    }

    static DictionaryField fetch(InputStream& stream)
    {
        string key = string::fetch(stream);
        T value = T::fetch(stream);
        return {key,
                value};
    }

    void store(OutputStream& stream) const
    {
        m_key.store(stream);
        m_value.store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const DictionaryField<T>& rhs)
        {
            return lhs.b_key == rhs.get_key()
                   && lhs.b_value == rhs.get_value();
        }

        Builder& set_key(const string::Builder& value) noexcept
        {
            b_key = value;
            return *this;
        }

        Builder& set_value(const T::Builder& value) noexcept
        {
            b_value = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            b_key.store(stream);
            b_value.store(stream);
        }

    private:
        string::Builder b_key;
        T::Builder b_value;
    };

private:
    string m_key;
    T m_value;
};

template <TLType T>
class Dictionary {
public:
    using TYPE_PARAM = T;
    static constexpr bool CTS = false;

    Dictionary() noexcept = default;

    Dictionary(const Dictionary&) noexcept = default;
    Dictionary(Dictionary&&) noexcept = default;
    Dictionary& operator=(const Dictionary&) noexcept = default;
    Dictionary& operator=(Dictionary&&) noexcept = default;

    ~Dictionary() noexcept = default;

    Dictionary(const array<DictionaryField<T>>& data) noexcept
        : m_data(data)
    {}

    const array<DictionaryField<T>>& get_data() const noexcept
    {
        return m_data;
    }

    static Dictionary fetch(InputStream& stream)
    {
        array<DictionaryField<T>> data = array<DictionaryField<T>>::fetch(stream);
        return {data};
    }

    void store(OutputStream& stream) const
    {
        m_data.store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const Dictionary<T>& rhs)
        {
            return lhs.b_data == rhs.get_data();
        }

        Builder& set_data(const array<DictionaryField<T>>::Builder& value) noexcept
        {
            b_data = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            b_data.store(stream);
        }

    private:
        array<DictionaryField<T>>::Builder b_data;
    };

private:
    array<DictionaryField<T>> m_data;
};

}    // namespace baseline
