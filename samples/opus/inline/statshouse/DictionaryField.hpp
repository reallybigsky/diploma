#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::inl::statshouse {

template <bool BOXED, TLType T>
class DictionaryField_BASE {
public:
    static constexpr Magic MAGIC = utils::commutative(3404569823, T::MAGIC);
    static constexpr bool STATIC = false;

    DictionaryField_BASE() noexcept = default;

    DictionaryField_BASE(const DictionaryField_BASE&) noexcept = default;
    DictionaryField_BASE(DictionaryField_BASE&&) noexcept = default;
    DictionaryField_BASE& operator=(const DictionaryField_BASE&) noexcept = default;
    DictionaryField_BASE& operator=(DictionaryField_BASE&&) noexcept = default;

    ~DictionaryField_BASE() noexcept = default;

    const string& get_key() const noexcept
    {
        return m_key;
    }

    const T& get_value() const noexcept
    {
        return m_value;
    }

    template <typename... ARGS>
    static DictionaryField_BASE fetch(InputStream& stream, ARGS&&... args)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        string key = string::fetch(stream);
        T value = T::fetch(stream, std::forward<ARGS>(args)...);
        DictionaryField_BASE result {std::move(key),
                                     std::move(value)};
        return result;
    }

    template <typename... ARGS>
    void store(OutputStream& stream, ARGS&&... args) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_key.store(stream);
        m_value.store(stream, std::forward<ARGS>(args)...);
    }

    class Builder {
    private:
        using TBuilder = typename T::Builder;

    public:
        using TYPE = DictionaryField_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const DictionaryField_BASE<RHS_BOXED, T>& rhs) noexcept
        {
            return lhs.b_key == rhs.get_key()
                   && lhs.b_value == rhs.get_value();
        }

        template <size_t KEY_SIZE>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_key(string::Builder::random<KEY_SIZE>(engine))
                    .set_value(TBuilder::random(engine));
        }

        template <size_t KEY_SIZE, size_t... SIZES>
        static Builder random(std::default_random_engine& engine) noexcept
        requires(requires { TBuilder::template random<SIZES...>(engine); })

        {
            return Builder {}
                    .set_key(string::Builder::random<KEY_SIZE>(engine))
                    .set_value(TBuilder::template random<SIZES...>(engine));
        }

        Builder& set_key(const string::Builder& value) noexcept
        {
            b_key = value;
            return *this;
        }

        Builder& set_value(const TBuilder& value) noexcept
        {
            b_value = value;
            return *this;
        }

        template <typename... Args>
        void store(OutputStream& stream, Args&&... args) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_key.store(stream);
            b_value.store(stream, std::forward<Args>(args)...);
        }

    private:
        string::Builder b_key;
        TBuilder b_value;
    };

private:
    DictionaryField_BASE(string&& key,
                         T&& value) noexcept
        : m_key(std::move(key))
        , m_value(std::move(value))
    {}

    string m_key;
    T m_value;
};

template <TLType T>
using dictionaryField = DictionaryField_BASE<false, T>;
template <TLType T>
using DictionaryField = DictionaryField_BASE<true, T>;

template <bool LHS_BOXED, bool RHS_BOXED, TLType T>
bool operator==(const DictionaryField_BASE<LHS_BOXED, T>& lhs, const DictionaryField_BASE<RHS_BOXED, T>& rhs) noexcept
{
    return lhs.get_key() == rhs.get_key()
           && lhs.get_value() == rhs.get_value();
}

template <bool BOXED, TLType T>
size_t consume(const DictionaryField_BASE<BOXED, T>& value) noexcept
{
    size_t result = 0;
    result += consume(value.get_key());
    result += consume(value.get_value());
    return result;
}

}    // namespace opus::inl::statshouse
