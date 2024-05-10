#pragma once

#include "prelude/opus/Includes.hpp"



namespace opus::proxy::statshouse {

class ReleaseChunkResponse {
public:
    static constexpr Magic MAGIC = 2512012688;

    ReleaseChunkResponse() noexcept = default;

    ReleaseChunkResponse(const ReleaseChunkResponse&) noexcept = default;
    ReleaseChunkResponse(ReleaseChunkResponse&&) noexcept = default;
    ReleaseChunkResponse& operator=(const ReleaseChunkResponse&) noexcept = default;
    ReleaseChunkResponse& operator=(ReleaseChunkResponse&&) noexcept = default;

    ~ReleaseChunkResponse() noexcept = default;

    bool verify() const noexcept
    {
        if (!get_fields_mask().verify())
            return false;

        if (!get_releasedChunkCount().verify())
            return false;

        return true;
    }

    Nat get_fields_mask() const noexcept
    {
        return m_fields_mask;
    }

    Int get_releasedChunkCount() const noexcept
    {
        return m_releasedChunkCount;
    }

    static ReleaseChunkResponse fetch(InputStream& stream)
    {
        Nat fields_mask = Nat::fetch(stream);
        Int releasedChunkCount = Int::fetch(stream);
        ReleaseChunkResponse result(std::move(fields_mask),
                                    std::move(releasedChunkCount));
        if (!result.verify()) throw TLException(TLException::TYPE::BAD_MAGIC);
        return result;
    }

    void store(OutputStream& stream) const
    {
        m_fields_mask.store(stream);
        m_releasedChunkCount.store(stream);
    }

    class Builder {
    public:
        friend bool operator==(const Builder& lhs, const ReleaseChunkResponse& rhs) noexcept
        {
            return lhs.b_fields_mask == rhs.get_fields_mask()
                   && lhs.b_releasedChunkCount == rhs.get_releasedChunkCount();
        }

        static Builder random(std::default_random_engine& engine) noexcept
        {
            return Builder {}
                    .set_fields_mask(Nat::Builder::random(engine))
                    .set_releasedChunkCount(Int::Builder::random(engine));
        }

        Builder& set_fields_mask(const Nat::Builder& value) noexcept
        {
            b_fields_mask = value;
            return *this;
        }

        Builder& set_releasedChunkCount(const Int::Builder& value) noexcept
        {
            b_releasedChunkCount = value;
            return *this;
        }

        void store(OutputStream& stream) const
        {
            MAGIC.store(stream);
            b_fields_mask.store(stream);
            b_releasedChunkCount.store(stream);
        }

    private:
        Nat::Builder b_fields_mask;
        Int::Builder b_releasedChunkCount;
    };

private:
    ReleaseChunkResponse(Nat&& fields_mask,
                         Int&& releasedChunkCount) noexcept
        : m_fields_mask(std::move(fields_mask))
        , m_releasedChunkCount(std::move(releasedChunkCount))
    {}

    Nat m_fields_mask;
    Int m_releasedChunkCount;
};

bool operator==(const ReleaseChunkResponse& lhs, const ReleaseChunkResponse& rhs) noexcept
{
    return lhs.get_fields_mask() == rhs.get_fields_mask()
           && lhs.get_releasedChunkCount() == rhs.get_releasedChunkCount();
}

}    // namespace opus::proxy::statshouse
