#pragma once

#include "prelude/opus/Includes.hpp"

#include "samples/opus/proxy_naive/statshouse/Dictionary.hpp"



namespace opus::proxy_naive::statshouse {

class HttpQuery {
public:
    static constexpr Magic MAGIC = 263205420;

    HttpQuery() noexcept = default;

    HttpQuery(const HttpQuery&) noexcept = default;
    HttpQuery(HttpQuery&&) noexcept = default;
    HttpQuery& operator=(const HttpQuery&) noexcept = default;
    HttpQuery& operator=(HttpQuery&&) noexcept = default;

    ~HttpQuery() noexcept = default;

    bool verify() const noexcept
    {
        if (!get_fields_mask().verify())
            return false;

        return true;
    }

    Nat get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    const std::optional<string>& get_uri() const noexcept
    {
        return m_uri;
    }

    const std::optional<dictionary<string>>& get_args() const noexcept
    {
        return m_args;
    }

    const std::optional<dictionary<string>>& get_headers() const noexcept
    {
        return m_headers;
    }

    static HttpQuery fetch(InputStream& stream)
    {
        Nat fields_mask = Nat::fetch(stream);
        std::optional<string> uri;
        if (IS_SET(fields_mask, 0)) uri = string::fetch(stream);
        std::optional<dictionary<string>> args;
        if (IS_SET(fields_mask, 1)) args = dictionary<string>::fetch(stream);
        std::optional<dictionary<string>> headers;
        if (IS_SET(fields_mask, 2)) headers = dictionary<string>::fetch(stream);
        HttpQuery result(std::move(fields_mask),
                         std::move(uri),
                         std::move(args),
                         std::move(headers));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_fields_mask.store(stream);
        if (IS_SET(get_fields_mask(), 0)) m_uri->store(stream);
        if (IS_SET(get_fields_mask(), 1)) m_args->store(stream);
        if (IS_SET(get_fields_mask(), 2)) m_headers->store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const HttpQuery& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && (!rhs.get_uri() || lhs.b_uri == rhs.get_uri())
                   && (!rhs.get_args() || lhs.b_args == rhs.get_args())
                   && (!rhs.get_headers() || lhs.b_headers == rhs.get_headers());
        }

        template <size_t SIZE_1, size_t SIZE_2, size_t SIZE_3, size_t SIZE_4, size_t SIZE_5, size_t SIZE_6, size_t SIZE_7>
        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(utils::random_mask<0, 7>(engine))
                    .set_uri(string::Builder::random<SIZE_1>(engine))
                    .set_args(dictionary<string>::Builder::random<SIZE_2, SIZE_3, SIZE_4>(engine))
                    .set_headers(dictionary<string>::Builder::random<SIZE_5, SIZE_6, SIZE_7>(engine));
        }

        Builder& set_fields_mask(const Nat::Builder& value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_uri(const string::Builder& value) noexcept
        {
            b_uri = value;
            return *this;
        }

        Builder& set_args(const dictionary<string>::Builder& value) noexcept
        {
            b_args = value;
            return *this;
        }

        Builder& set_headers(const dictionary<string>::Builder& value) noexcept
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
        dictionary<string>::Builder b_args;
        dictionary<string>::Builder b_headers;
    };

private:
    HttpQuery(Nat&& fields_mask,
              std::optional<string>&& uri,
              std::optional<dictionary<string>>&& args,
              std::optional<dictionary<string>>&& headers) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_uri(std::move(uri))
        , m_args(std::move(args))
        , m_headers(std::move(headers))
    {}

    Nat m_fields_mask;
    std::optional<string> m_uri;
    std::optional<dictionary<string>> m_args;
    std::optional<dictionary<string>> m_headers;
};

bool operator==(const HttpQuery& lhs, const HttpQuery& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_uri() == rhs.get_uri()
           && lhs.get_args() == rhs.get_args()
           && lhs.get_headers() == rhs.get_headers();
}

size_t consume(const HttpQuery& value) noexcept
{
    size_t result = 0;
    result += consume(value.get_fields_mask());
    if (value.get_uri()) result += consume(*value.get_uri());
    if (value.get_args()) result += consume(*value.get_args());
    if (value.get_headers()) result += consume(*value.get_headers());
    return result;
}

}    // namespace opus::proxy_naive::statshouse
