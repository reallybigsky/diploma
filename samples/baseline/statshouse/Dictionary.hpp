#pragma once

#include "prelude/baseline/Includes.hpp"

#include "samples/baseline/statshouse/DictionaryField.hpp"



namespace baseline::statshouse {

template <bool BOXED, TLType T>
class Dictionary_BASE {
public:
    static constexpr Magic MAGIC = utils::commutative(2898633699, T::MAGIC);
    static constexpr bool STATIC = false;

    Dictionary_BASE() noexcept = default;

    Dictionary_BASE(const Dictionary_BASE&) noexcept = default;
    Dictionary_BASE(Dictionary_BASE&&) noexcept = default;
    Dictionary_BASE& operator=(const Dictionary_BASE&) noexcept = default;
    Dictionary_BASE& operator=(Dictionary_BASE&&) noexcept = default;

    ~Dictionary_BASE() noexcept = default;

    const array<dictionaryField<T>>& get_array1() const noexcept
    {
        return m_array1;
    }

    template <typename... ARGS>
    static Dictionary_BASE fetch(InputStream& stream, ARGS&&... args)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        array<dictionaryField<T>> array1 = array<dictionaryField<T>>::fetch(stream, std::forward<ARGS>(args)...);
        Dictionary_BASE result {std::move(array1)};
        return result;
    }

    template <typename... ARGS>
    void store(OutputStream& stream, ARGS&&... args) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_array1.store(stream, std::forward<ARGS>(args)...);
    }

    class Builder {
    public:
        using TYPE = Dictionary_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const Dictionary_BASE<RHS_BOXED, T>& rhs) noexcept
        {
            return lhs.b_array1 == rhs.get_array1();
        }

        template <size_t... SIZES>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_array1(array<dictionaryField<T>>::Builder::template random<SIZES...>(engine));
        }

        Builder& set_array1(const array<dictionaryField<T>>::Builder& value) noexcept
        {
            b_array1 = value;
            return *this;
        }

        template <typename... ARGS>
        void store(OutputStream& stream, ARGS&&... args) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_array1.store(stream, std::forward<ARGS>(args)...);
        }

    private:
        array<dictionaryField<T>>::Builder b_array1;
    };

private:
    Dictionary_BASE(array<dictionaryField<T>>&& array1) noexcept
        : m_array1(std::move(array1))
    {}

    array<dictionaryField<T>> m_array1;
};

template <TLType T>
using dictionary = Dictionary_BASE<false, T>;
template <TLType T>
using Dictionary = Dictionary_BASE<true, T>;

template <bool LHS_BOXED, bool RHS_BOXED, TLType T>
bool operator==(const Dictionary_BASE<LHS_BOXED, T>& lhs, const Dictionary_BASE<RHS_BOXED, T>& rhs) noexcept
{
    return lhs.get_array1() == rhs.get_array1();
}

}    // namespace baseline::statshouse
