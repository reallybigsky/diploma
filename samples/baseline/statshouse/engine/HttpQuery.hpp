#pragma once

#include "baseline/Includes.hpp"
#include "baseline/statshouse/Dictionary.hpp"



namespace baseline::statshouse {

class HttpQuery {
public:
    static constexpr ACCESS ACCESS_BY = ACCESS::REF;
    static constexpr Magic MAGIC = 263205420;
    static constexpr bool CTS = false;

    HttpQuery() noexcept = default;

    HttpQuery(const HttpQuery&) noexcept = default;
    HttpQuery(HttpQuery&&) noexcept = default;
    HttpQuery& operator=(const HttpQuery&) noexcept = default;
    HttpQuery& operator=(HttpQuery&&) noexcept = default;

    ~HttpQuery() noexcept = default;

    HttpQuery(Nat fields_mask,
              const string& uri,
              const Dictionary<string>& args,
              const Dictionary<string>& headers) noexcept
        : m_fields_mask(fields_mask)
        , m_uri(uri)
        , m_args(args)
        , m_headers(headers)
    {}

    Nat get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const string& get_uri() const noexcept
    {
        return m_uri;
    }

    const Dictionary<string>& get_args() const noexcept
    {
        return m_args;
    }

    const Dictionary<string>& get_headers() const noexcept
    {
        return m_headers;
    }

    static HttpQuery fetch(InputStream& stream)
    {
        Magic magic = Magic::fetch(stream);
        if (magic != MAGIC) throw TLException(TLException::TYPE::BAD_MAGIC);

        Nat fields_mask = Nat::fetch(stream);
        string uri;
        if (IS_SET(fields_mask, 0)) uri = string::fetch(stream);
        Dictionary<string> args;
        if (IS_SET(fields_mask, 1)) args = Dictionary<string>::fetch(stream);
        Dictionary<string> headers;
        if (IS_SET(fields_mask, 2)) headers = Dictionary<string>::fetch(stream);
        return {fields_mask,
                uri,
                args,
                headers};
    }

    void store(OutputStream& stream) const
    {
        MAGIC.store(stream);
        m_fields_mask.store(stream);
        if (IS_SET(m_fields_mask, 0)) m_uri.store(stream);
        if (IS_SET(m_fields_mask, 1)) m_args.store(stream);
        if (IS_SET(m_fields_mask, 2)) m_headers.store(stream);
    }

    class Builder {
    public:
        Builder& set_fields_mask(Nat::Builder value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_uri(const string::Builder& value) noexcept
        {
            b_uri = value;
            return *this;
        }

        Builder& set_args(const Dictionary<string>::Builder& value) noexcept
        {
            b_args = value;
            return *this;
        }

        Builder& set_headers(const Dictionary<string>::Builder& value) noexcept
        {
            b_headers = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            MAGIC.store(stream);
            b_fields_mask.store(stream);
            if (IS_SET(b_fields_mask, 0)) b_uri.store(stream);
            if (IS_SET(b_fields_mask, 1)) b_args.store(stream);
            if (IS_SET(b_fields_mask, 2)) b_headers.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        string::Builder b_uri;
        Dictionary<string>::Builder b_args;
        Dictionary<string>::Builder b_headers;
    };

private:
    Nat m_fields_mask;
    string m_uri;
    Dictionary<string> m_args;
    Dictionary<string> m_headers;
};

}    // namespace baseline::statshouse
