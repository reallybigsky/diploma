#pragma once

#include "baseline/Includes.hpp"
#include "baseline/statshouse/Dictionary.hpp"



namespace baseline::statshouse {

class HttpQueryResponse {
public:
    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
    static constexpr Magic MAGIC = 1808768776;
    static constexpr bool CTS = false;

    HttpQueryResponse() noexcept = default;

    HttpQueryResponse(const HttpQueryResponse&) noexcept = default;
    HttpQueryResponse(HttpQueryResponse&&) noexcept = default;
    HttpQueryResponse& operator=(const HttpQueryResponse&) noexcept = default;
    HttpQueryResponse& operator=(HttpQueryResponse&&) noexcept = default;

    ~HttpQueryResponse() noexcept = default;

    HttpQueryResponse(Nat fields_mask,
                      Int return_code,
                      const string& data,
                      const string& content_type,
                      const Dictionary<string>& additional_headers) noexcept
        : m_fields_mask(fields_mask)
        , m_return_code(return_code)
        , m_data(data)
        , m_content_type(content_type)
        , m_additional_headers(additional_headers)
    {}

    Nat get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    Int get_return_code() const noexcept
    {
        return m_return_code;
    }

    const string& get_data() const noexcept
    {
        return m_data;
    }

    const string& get_content_type() const noexcept
    {
        return m_content_type;
    }

    const Dictionary<string>& get_additional_headers() const noexcept
    {
        return m_additional_headers;
    }

    static HttpQueryResponse fetch(InputStream& stream)
    {
        Magic magic = Magic::fetch(stream);
        if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);

        Nat fields_mask = Nat::fetch(stream);
        Int return_code;
        if (IS_SET(fields_mask, 0)) return_code = Int::fetch(stream);
        string data;
        if (IS_SET(fields_mask, 1)) data = string::fetch(stream);
        string content_type;
        if (IS_SET(fields_mask, 2)) content_type = string::fetch(stream);
        Dictionary<string> additional_headers;
        if (IS_SET(fields_mask, 3)) additional_headers = Dictionary<string>::fetch(stream);
        return {fields_mask,
                return_code,
                data,
                content_type,
                additional_headers};
    }

    void store(OutputStream& stream) const
    {
        MAGIC.store(stream);
        m_fields_mask.store(stream);
        if (IS_SET(m_fields_mask, 0)) m_return_code.store(stream);
        if (IS_SET(m_fields_mask, 1)) m_data.store(stream);
        if (IS_SET(m_fields_mask, 2)) m_content_type.store(stream);
        if (IS_SET(m_fields_mask, 3)) m_additional_headers.store(stream);
    }

    class Builder {
    public:
        Builder& set_fields_mask(Nat::Builder value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_return_code(Int::Builder value) noexcept
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

        Builder& set_additional_headers(const Dictionary<string>::Builder& value) noexcept
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
        Dictionary<string>::Builder b_additional_headers;
    };

private:
    Nat m_fields_mask;
    Int m_return_code;
    string m_data;
    string m_content_type;
    Dictionary<string> m_additional_headers;
};

}    // namespace baseline::statshouse
