#pragma once

#include "prelude/baseline/Includes.hpp"

#include "samples/baseline/statshouse/Dictionary.hpp"



namespace baseline::statshouse {

class HttpQueryResponse {
public:
    static constexpr Magic MAGIC = 1808768776;

    HttpQueryResponse() noexcept = default;

    HttpQueryResponse(const HttpQueryResponse&) noexcept = default;
    HttpQueryResponse(HttpQueryResponse&&) noexcept = default;
    HttpQueryResponse& operator=(const HttpQueryResponse&) noexcept = default;
    HttpQueryResponse& operator=(HttpQueryResponse&&) noexcept = default;

    ~HttpQueryResponse() noexcept = default;

    const Nat& get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const std::optional<Int>& get_return_code() const noexcept
    {
        return m_return_code;
    }

    const std::optional<string>& get_data() const noexcept
    {
        return m_data;
    }

    const std::optional<string>& get_content_type() const noexcept
    {
        return m_content_type;
    }

    const std::optional<dictionary<string>>& get_additional_headers() const noexcept
    {
        return m_additional_headers;
    }

    static HttpQueryResponse fetch(InputStream& stream)
    {
        Nat fields_mask = Nat::fetch(stream);
        std::optional<Int> return_code;
        if (IS_SET(fields_mask, 0)) return_code = Int::fetch(stream);
        std::optional<string> data;
        if (IS_SET(fields_mask, 1)) data = string::fetch(stream);
        std::optional<string> content_type;
        if (IS_SET(fields_mask, 2)) content_type = string::fetch(stream);
        std::optional<dictionary<string>> additional_headers;
        if (IS_SET(fields_mask, 3)) additional_headers = dictionary<string>::fetch(stream);
        HttpQueryResponse result(std::move(fields_mask),
                                 std::move(return_code),
                                 std::move(data),
                                 std::move(content_type),
                                 std::move(additional_headers));
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_fields_mask.store(stream);
        if (IS_SET(get_fields_mask(), 0)) m_return_code->store(stream);
        if (IS_SET(get_fields_mask(), 1)) m_data->store(stream);
        if (IS_SET(get_fields_mask(), 2)) m_content_type->store(stream);
        if (IS_SET(get_fields_mask(), 3)) m_additional_headers->store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const HttpQueryResponse& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && (!rhs.get_return_code() || lhs.b_return_code == rhs.get_return_code())
                   && (!rhs.get_data() || lhs.b_data == rhs.get_data())
                   && (!rhs.get_content_type() || lhs.b_content_type == rhs.get_content_type())
                   && (!rhs.get_additional_headers() || lhs.b_additional_headers == rhs.get_additional_headers());
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3, size_t SIZE_4, size_t SIZE_5>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(utils::random_mask<0, 15>(engine))
                    .set_return_code(Int::Builder::random(engine))
                    .set_data(string::Builder::random<SIZE_1>(engine))
                    .set_content_type(string::Builder::random<SIZE_2>(engine))
                    .set_additional_headers(dictionary<string>::Builder::random<SIZE_3, SIZE_4, SIZE_5>(engine));
        }

        Builder& set_fields_mask(const Nat::Builder& value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_return_code(const Int::Builder& value) noexcept
        {
            b_return_code = value;
            return *this;
        }

        Builder& set_data(const string::Builder& value) noexcept
        {
            b_data = value;
            return *this;
        }

        Builder& set_content_type(const string::Builder& value) noexcept
        {
            b_content_type = value;
            return *this;
        }

        Builder& set_additional_headers(const dictionary<string>::Builder& value) noexcept
        {
            b_additional_headers = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            MAGIC.store(stream);
            b_fields_mask.store(stream);
            if (IS_SET(b_fields_mask, 0)) b_return_code.store(stream);
            if (IS_SET(b_fields_mask, 1)) b_data.store(stream);
            if (IS_SET(b_fields_mask, 2)) b_content_type.store(stream);
            if (IS_SET(b_fields_mask, 3)) b_additional_headers.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        Int::Builder b_return_code;
        string::Builder b_data;
        string::Builder b_content_type;
        dictionary<string>::Builder b_additional_headers;
    };

private:
    HttpQueryResponse(Nat&& fields_mask,
                      std::optional<Int>&& return_code,
                      std::optional<string>&& data,
                      std::optional<string>&& content_type,
                      std::optional<dictionary<string>>&& additional_headers) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_return_code(std::move(return_code))
        , m_data(std::move(data))
        , m_content_type(std::move(content_type))
        , m_additional_headers(std::move(additional_headers))
    {}

    Nat m_fields_mask;
    std::optional<Int> m_return_code;
    std::optional<string> m_data;
    std::optional<string> m_content_type;
    std::optional<dictionary<string>> m_additional_headers;
};

bool operator==(const HttpQueryResponse& lhs, const HttpQueryResponse& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_return_code() == rhs.get_return_code()
           && lhs.get_data() == rhs.get_data()
           && lhs.get_content_type() == rhs.get_content_type()
           && lhs.get_additional_headers() == rhs.get_additional_headers();
}

size_t consume(const HttpQueryResponse& value) noexcept
{
    size_t result = 0;
    result += consume(value.get_fields_mask());
    if (value.get_return_code()) result += consume(*value.get_return_code());
    if (value.get_data()) result += consume(*value.get_data());
    if (value.get_content_type()) result += consume(*value.get_content_type());
    if (value.get_additional_headers()) result += consume(*value.get_additional_headers());
    return result;
}

}    // namespace baseline::statshouse
