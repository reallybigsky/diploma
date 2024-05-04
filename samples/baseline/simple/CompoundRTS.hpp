#pragma once

#include "prelude/baseline/Includes.hpp"

#include "samples/baseline/simple/CompoundCTS.hpp"
#include "samples/baseline/simple/IntDouble.hpp"
#include "samples/baseline/simple/IntDoubleLong.hpp"
#include "samples/baseline/simple/StringIntLong.hpp"



namespace baseline::simple {

template <bool BOXED>
class CompoundRTS_BASE {
public:
    static constexpr Magic MAGIC = COMPILE_TIME_CRC32_STR("CompoundRTS");
    static constexpr bool STATIC = false;

    CompoundRTS_BASE() noexcept = default;

    CompoundRTS_BASE(const CompoundRTS_BASE&) noexcept = default;
    CompoundRTS_BASE(CompoundRTS_BASE&&) noexcept = default;
    CompoundRTS_BASE& operator=(const CompoundRTS_BASE&) noexcept = default;
    CompoundRTS_BASE& operator=(CompoundRTS_BASE&&) noexcept = default;

    ~CompoundRTS_BASE() noexcept = default;

    const Array<Int>& get_Array1() const noexcept
    {
        return m_Array1;
    }

    const Int& get_Int1() const noexcept
    {
        return m_Int1;
    }

    const Long& get_Long1() const noexcept
    {
        return m_Long1;
    }

    const String& get_String1() const noexcept
    {
        return m_String1;
    }

    const IntDouble& get_IntDouble1() const noexcept
    {
        return m_IntDouble1;
    }

    const StringIntLong& get_StringIntLong1() const noexcept
    {
        return m_StringIntLong1;
    }

    const String& get_String2() const noexcept
    {
        return m_String2;
    }

    const Double& get_Double1() const noexcept
    {
        return m_Double1;
    }

    const Double& get_Double2() const noexcept
    {
        return m_Double2;
    }

    const Double& get_Double3() const noexcept
    {
        return m_Double3;
    }

    const Array<IntDoubleLong>& get_Array2() const noexcept
    {
        return m_Array2;
    }

    const CompoundCTS& get_CompoundCTS1() const noexcept
    {
        return m_CompoundCTS1;
    }

    const Float& get_Float1() const noexcept
    {
        return m_Float1;
    }

    const IntDouble& get_IntDouble2() const noexcept
    {
        return m_IntDouble2;
    }

    const IntDouble& get_IntDouble3() const noexcept
    {
        return m_IntDouble3;
    }

    static CompoundRTS_BASE fetch(InputStream& stream)
    {
        if constexpr (BOXED) {
            Magic magic = Magic::fetch(stream);
            if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);
        }
        Array<Int> Array1 = Array<Int>::fetch(stream);
        Int Int1 = Int::fetch(stream);
        Long Long1 = Long::fetch(stream);
        String String1 = String::fetch(stream);
        IntDouble IntDouble1 = IntDouble::fetch(stream);
        StringIntLong StringIntDouble1 = StringIntLong::fetch(stream);
        String String2 = String::fetch(stream);
        Double Double1 = Double::fetch(stream);
        Double Double2 = Double::fetch(stream);
        Double Double3 = Double::fetch(stream);
        Array<IntDoubleLong> Array2 = Array<IntDoubleLong>::fetch(stream);
        CompoundCTS CompoundCTS1 = CompoundCTS::fetch(stream);
        Float Float1 = Float::fetch(stream);
        IntDouble IntDouble2 = IntDouble::fetch(stream);
        IntDouble IntDouble3 = IntDouble::fetch(stream);
        CompoundRTS_BASE result(std::move(Array1),
                                std::move(Int1),
                                std::move(Long1),
                                std::move(String1),
                                std::move(IntDouble1),
                                std::move(StringIntDouble1),
                                std::move(String2),
                                std::move(Double1),
                                std::move(Double2),
                                std::move(Double3),
                                std::move(Array2),
                                std::move(CompoundCTS1),
                                std::move(Float1),
                                std::move(IntDouble2),
                                std::move(IntDouble3));
        return result;
    }

    void store(OutputStream& stream) const
    {
        if constexpr (BOXED) {
            MAGIC.store(stream);
        }
        m_Array1.store(stream);
        m_Int1.store(stream);
        m_Long1.store(stream);
        m_String1.store(stream);
        m_IntDouble1.store(stream);
        m_StringIntLong1.store(stream);
        m_String2.store(stream);
        m_Double1.store(stream);
        m_Double2.store(stream);
        m_Double3.store(stream);
        m_Array2.store(stream);
        m_CompoundCTS1.store(stream);
        m_Float1.store(stream);
        m_IntDouble2.store(stream);
        m_IntDouble3.store(stream);
    }

    class Builder {
    public:
        using TYPE = CompoundRTS_BASE;

        template <bool RHS_BOXED>
        friend bool operator==(const Builder& lhs, const CompoundRTS_BASE<RHS_BOXED>& rhs) noexcept
        {
            return lhs.b_Array1 == rhs.get_Array1()
                   && lhs.b_Int1 == rhs.get_Int1()
                   && lhs.b_Long1 == rhs.get_Long1()
                   && lhs.b_String1 == rhs.get_String1()
                   && lhs.b_IntDouble1 == rhs.get_IntDouble1()
                   && lhs.b_StringIntLong1 == rhs.get_StringIntLong1()
                   && lhs.b_String2 == rhs.get_String2()
                   && lhs.b_Double1 == rhs.get_Double1()
                   && lhs.b_Double2 == rhs.get_Double2()
                   && lhs.b_Double3 == rhs.get_Double3()
                   && lhs.b_Array2 == rhs.get_Array2()
                   && lhs.b_CompoundCTS1 == rhs.get_CompoundCTS1()
                   && lhs.b_Float1 == rhs.get_Float1()
                   && lhs.b_IntDouble2 == rhs.get_IntDouble2()
                   && lhs.b_IntDouble3 == rhs.get_IntDouble3();
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3, size_t SIZE_4, size_t SIZE_5>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_Array1(Array<Int>::Builder::random<SIZE_1>(engine))
                    .set_Int1(Int::Builder::random(engine))
                    .set_Long1(Long::Builder::random(engine))
                    .set_String1(String::Builder::random<SIZE_2>(engine))
                    .set_IntDouble1(IntDouble::Builder::random(engine))
                    .set_StringIntLong1(StringIntLong::Builder::random<SIZE_3>(engine))
                    .set_String2(String::Builder::random<SIZE_4>(engine))
                    .set_Double1(Double::Builder::random(engine))
                    .set_Double2(Double::Builder::random(engine))
                    .set_Double3(Double::Builder::random(engine))
                    .set_Array2(Array<IntDoubleLong>::Builder::random<SIZE_5>(engine))
                    .set_CompoundCTS1(CompoundCTS::Builder::random(engine))
                    .set_Float1(Float::Builder::random(engine))
                    .set_IntDouble2(IntDouble::Builder::random(engine))
                    .set_IntDouble3(IntDouble::Builder::random(engine));
        }

        Builder& set_Array1(const Array<Int>::Builder& value) noexcept
        {
            b_Array1 = value;
            return *this;
        }

        Builder& set_Int1(const Int::Builder& value) noexcept
        {
            b_Int1 = value;
            return *this;
        }

        Builder& set_Long1(const Long::Builder& value) noexcept
        {
            b_Long1 = value;
            return *this;
        }

        Builder& set_String1(const String::Builder& value) noexcept
        {
            b_String1 = value;
            return *this;
        }

        Builder& set_IntDouble1(const IntDouble::Builder& value) noexcept
        {
            b_IntDouble1 = value;
            return *this;
        }

        Builder& set_StringIntLong1(const StringIntLong::Builder& value) noexcept
        {
            b_StringIntLong1 = value;
            return *this;
        }

        Builder& set_String2(const String::Builder& value) noexcept
        {
            b_String2 = value;
            return *this;
        }

        Builder& set_Double1(const Double::Builder& value) noexcept
        {
            b_Double1 = value;
            return *this;
        }

        Builder& set_Double2(const Double::Builder& value) noexcept
        {
            b_Double2 = value;
            return *this;
        }

        Builder& set_Double3(const Double::Builder& value) noexcept
        {
            b_Double3 = value;
            return *this;
        }

        Builder& set_Array2(const Array<IntDoubleLong>::Builder& value) noexcept
        {
            b_Array2 = value;
            return *this;
        }

        Builder& set_CompoundCTS1(const CompoundCTS::Builder& value) noexcept
        {
            b_CompoundCTS1 = value;
            return *this;
        }

        Builder& set_Float1(const Float::Builder& value) noexcept
        {
            b_Float1 = value;
            return *this;
        }

        Builder& set_IntDouble2(const IntDouble::Builder& value) noexcept
        {
            b_IntDouble2 = value;
            return *this;
        }

        Builder& set_IntDouble3(const IntDouble::Builder& value) noexcept
        {
            b_IntDouble3 = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            if constexpr (BOXED) {
                MAGIC.store(stream);
            }
            b_Array1.store(stream);
            b_Int1.store(stream);
            b_Long1.store(stream);
            b_String1.store(stream);
            b_IntDouble1.store(stream);
            b_StringIntLong1.store(stream);
            b_String2.store(stream);
            b_Double1.store(stream);
            b_Double2.store(stream);
            b_Double3.store(stream);
            b_Array2.store(stream);
            b_CompoundCTS1.store(stream);
            b_Float1.store(stream);
            b_IntDouble2.store(stream);
            b_IntDouble3.store(stream);
        }

    private:
        Array<Int>::Builder b_Array1;
        Int::Builder b_Int1;
        Long::Builder b_Long1;
        String::Builder b_String1;
        IntDouble::Builder b_IntDouble1;
        StringIntLong::Builder b_StringIntLong1;
        String::Builder b_String2;
        Double::Builder b_Double1;
        Double::Builder b_Double2;
        Double::Builder b_Double3;
        Array<IntDoubleLong>::Builder b_Array2;
        CompoundCTS::Builder b_CompoundCTS1;
        Float::Builder b_Float1;
        IntDouble::Builder b_IntDouble2;
        IntDouble::Builder b_IntDouble3;
    };

private:
    CompoundRTS_BASE(Array<Int>&& Array1,
                     Int&& Int1,
                     Long&& Long1,
                     String&& String1,
                     IntDouble&& IntDouble1,
                     StringIntLong&& StringIntLong1,
                     String&& String2,
                     Double&& Double1,
                     Double&& Double2,
                     Double&& Double3,
                     Array<IntDoubleLong>&& Array2,
                     CompoundCTS&& CompoundCTS1,
                     Float&& Float1,
                     IntDouble&& IntDouble2,
                     IntDouble&& IntDouble3) noexcept
        : m_Array1(std::move(Array1))
        , m_Int1(std::move(Int1))
        , m_Long1(std::move(Long1))
        , m_String1(std::move(String1))
        , m_IntDouble1(std::move(IntDouble1))
        , m_StringIntLong1(std::move(StringIntLong1))
        , m_String2(std::move(String2))
        , m_Double1(std::move(Double1))
        , m_Double2(std::move(Double2))
        , m_Double3(std::move(Double3))
        , m_Array2(std::move(Array2))
        , m_CompoundCTS1(std::move(CompoundCTS1))
        , m_Float1(std::move(Float1))
        , m_IntDouble2(std::move(IntDouble2))
        , m_IntDouble3(std::move(IntDouble3))
    {}

    Array<Int> m_Array1;
    Int m_Int1;
    Long m_Long1;
    String m_String1;
    IntDouble m_IntDouble1;
    StringIntLong m_StringIntLong1;
    String m_String2;
    Double m_Double1;
    Double m_Double2;
    Double m_Double3;
    Array<IntDoubleLong> m_Array2;
    CompoundCTS m_CompoundCTS1;
    Float m_Float1;
    IntDouble m_IntDouble2;
    IntDouble m_IntDouble3;
};

using compoundRTS = CompoundRTS_BASE<false>;
using CompoundRTS = CompoundRTS_BASE<true>;

template <bool LHS_BOXED, bool RHS_BOXED>
bool operator==(const CompoundRTS_BASE<LHS_BOXED>& lhs, const CompoundRTS_BASE<RHS_BOXED>& rhs) noexcept
{
    return lhs.get_Array1() == rhs.get_Array1()
           && lhs.get_Int1() == rhs.get_Int1()
           && lhs.get_Long1() == rhs.get_Long1()
           && lhs.get_String1() == rhs.get_String1()
           && lhs.get_IntDouble1() == rhs.get_IntDouble1()
           && lhs.get_StringIntLong1() == rhs.get_StringIntLong1()
           && lhs.get_String2() == rhs.get_String2()
           && lhs.get_Double1() == rhs.get_Double1()
           && lhs.get_Double2() == rhs.get_Double2()
           && lhs.get_Double3() == rhs.get_Double3()
           && lhs.get_Array2() == rhs.get_Array2()
           && lhs.get_CompoundCTS1() == rhs.get_CompoundCTS1()
           && lhs.get_Float1() == rhs.get_Float1()
           && lhs.get_IntDouble2() == rhs.get_IntDouble2()
           && lhs.get_IntDouble3() == rhs.get_IntDouble3();
}

}    // namespace baseline::simple
