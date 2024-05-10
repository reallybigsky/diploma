#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::proxy_naive::statshouse {

class Function : private Magic {
public:
    static constexpr Magic MAGIC = 1005992323;
    static constexpr size_t SIZEOF = Magic::SIZEOF;
    static constexpr bool STATIC = true;

    enum Value : uint32_t {
        fnCount = 4244540513,
        fnCountNorm = 3670563159,
        fnCumulCount = 3983780165,
        fnMin = 3622919900,
        fnMax = 1814528868,
        fnAvg = 2296577293,
        fnCumulAvg = 3800420384,
        fnSum = 2906413796,
        fnSumNorm = 259945234,
        fnCumulSum = 2702324593,
        fnStddev = 2834221755,
    };

    Function() noexcept = default;

    Function(const Function&) noexcept = default;
    Function(Function&&) noexcept = default;
    Function& operator=(const Function&) noexcept = default;
    Function& operator=(Function&&) noexcept = default;

    ~Function() noexcept = default;

    Function(const Proxy& proxy, offset_t offset) noexcept
        : Magic(proxy, offset)
    {}

    bool verify() const noexcept
    {
        switch (value) {
            case Value::fnCount:
                return true;
            case Value::fnCountNorm:
                return true;
            case Value::fnCumulCount:
                return true;
            case Value::fnMin:
                return true;
            case Value::fnMax:
                return true;
            case Value::fnAvg:
                return true;
            case Value::fnCumulAvg:
                return true;
            case Value::fnSum:
                return true;
            case Value::fnSumNorm:
                return true;
            case Value::fnCumulSum:
                return true;
            case Value::fnStddev:
                return true;
            default:
                return false;
        }
    }

    Value get() const noexcept
    {
        return Value {Magic::value};
    }

    static Function fetch(InputStream& stream)
    {
        Magic magic = Magic::fetch(stream);
        switch (magic) {
            case Value::fnCount:
                return Function {Value::fnCount};
            case Value::fnCountNorm:
                return Function {Value::fnCountNorm};
            case Value::fnCumulCount:
                return Function {Value::fnCumulCount};
            case Value::fnMin:
                return Function {Value::fnMin};
            case Value::fnMax:
                return Function {Value::fnMax};
            case Value::fnAvg:
                return Function {Value::fnAvg};
            case Value::fnCumulAvg:
                return Function {Value::fnCumulAvg};
            case Value::fnSum:
                return Function {Value::fnSum};
            case Value::fnSumNorm:
                return Function {Value::fnSumNorm};
            case Value::fnCumulSum:
                return Function {Value::fnCumulSum};
            case Value::fnStddev:
                return Function {Value::fnStddev};
            default:
                throw TLException(TLException::TYPE::BAD_MAGIC);
        }
    }

    void store(OutputStream& stream) const
    {
        Magic::store(stream);
    }

    friend bool operator==(const Function&, const Function&) noexcept = default;

    class Builder {
    public:
        using TYPE = Function;

        friend bool operator==(const Builder& lhs, const Function& rhs) noexcept
        {
            return lhs.b_value == rhs.get();
        }

        static Builder random(std::default_random_engine& engine) noexcept
        {
            Value values[] = {Value::fnCount,
                              Value::fnCountNorm,
                              Value::fnCumulCount,
                              Value::fnMin,
                              Value::fnMax,
                              Value::fnAvg,
                              Value::fnCumulAvg,
                              Value::fnSum,
                              Value::fnSumNorm,
                              Value::fnCumulSum,
                              Value::fnStddev};
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
    Function(Magic&& value) noexcept
        : Magic(std::move(value))
    {}
};

}    // namespace opus::proxy_naive::statshouse
