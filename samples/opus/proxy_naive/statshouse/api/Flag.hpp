#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::proxy_naive::statshouse {

class Flag : private Magic {
public:
    static constexpr Magic MAGIC = 2583724524;
    static constexpr size_t SIZEOF = Magic::SIZEOF;
    static constexpr bool STATIC = true;

    enum Value : uint32_t {
        flagMapped = 911274610,
        flagRaw = 766947422,
        flagAuto = 3423974622,
    };

    Flag() noexcept = default;

    Flag(const Flag&) noexcept = default;
    Flag(Flag&&) noexcept = default;
    Flag& operator=(const Flag&) noexcept = default;
    Flag& operator=(Flag&&) noexcept = default;

    ~Flag() noexcept = default;

    Flag(const Proxy& proxy, offset_t offset) noexcept
        : Magic(proxy, offset)
    {}

    bool verify() const noexcept
    {
        switch (value) {
            case Value::flagMapped:
                return true;
            case Value::flagRaw:
                return true;
            case Value::flagAuto:
                return true;
            default:
                return false;
        }
    }

    Value get() const noexcept
    {
        return Value {Magic::value};
    }

    static Flag fetch(InputStream& stream)
    {
        Magic magic = Magic::fetch(stream);
        switch (magic) {
            case Value::flagMapped:
                return Flag {Value::flagMapped};
            case Value::flagRaw:
                return Flag {Value::flagRaw};
            case Value::flagAuto:
                return Flag {Value::flagAuto};
            default:
                throw TLException(TLException::TYPE::BAD_MAGIC);
        }
    }

    void store(OutputStream& stream) const
    {
        Magic::store(stream);
    }

    friend bool operator==(const Flag&, const Flag&) noexcept = default;

    class Builder {
    public:
        using TYPE = Flag;

        friend bool operator==(const Builder& lhs, const Flag& rhs) noexcept
        {
            return lhs.b_value == rhs.get();
        }

        static Builder random(std::default_random_engine& engine) noexcept
        {
            Value values[] = {Value::flagMapped,
                              Value::flagRaw,
                              Value::flagAuto};
            static std::uniform_int_distribution<uint32_t> ud(0, sizeof(values) / sizeof(Value) - 1);
            return Builder {}
                    .set(values[ud(engine)]);
        }

        Builder& set(const Value& value) noexcept
        {
            b_value = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            stream.store<uint32_t>(b_value);
        }

    private:
        Value b_value;
    };

private:
    Flag(Magic&& value) noexcept
        : Magic(std::move(value))
    {}
};

size_t consume(const Flag& value) noexcept
{
    size_t result = 0;
    result += value.get();
    return result;
}

}    // namespace opus::proxy_naive::statshouse
