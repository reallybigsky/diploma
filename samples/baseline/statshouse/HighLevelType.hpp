#pragma once

#include "prelude/baseline/Includes.hpp"

#include "samples/baseline/statshouse/api/GetChunkResponse.hpp"
#include "samples/baseline/statshouse/api/GetQueryPointResponse.hpp"
#include "samples/baseline/statshouse/api/GetQueryResponse.hpp"
#include "samples/baseline/statshouse/api/Query.hpp"
#include "samples/baseline/statshouse/api/QueryPoint.hpp"
#include "samples/baseline/statshouse/api/ReleaseChunkResponse.hpp"
#include "samples/baseline/statshouse/engine/HttpQuery.hpp"
#include "samples/baseline/statshouse/engine/HttpQueryResponse.hpp"
#include "samples/baseline/statshouse/public/AddMetricsBatchQuery.hpp"
#include "samples/baseline/statshouse/schema/SourceBucket2.hpp"

#include <variant>



namespace baseline::statshouse {

class HighLevelType {
public:
    static constexpr Magic MAGIC = 3440603448;
    static constexpr bool STATIC = false;

    using Variant = std::variant<AddMetricsBatchQuery,
                                 HttpQuery,
                                 HttpQueryResponse,
                                 Query,
                                 QueryPoint,
                                 GetQueryResponse,
                                 GetQueryPointResponse,
                                 GetChunkResponse,
                                 ReleaseChunkResponse,
                                 SourceBucket2>;

    const Variant& get() const noexcept
    {
        return m_data;
    }

    static HighLevelType fetch(InputStream& stream)
    {
        Magic magic = Magic::fetch(stream);
        switch (magic) {
            case AddMetricsBatchQuery::MAGIC:
                return {Variant {AddMetricsBatchQuery::fetch(stream)}};
            case HttpQuery::MAGIC:
                return {Variant {HttpQuery::fetch(stream)}};
            case HttpQueryResponse::MAGIC:
                return {Variant {HttpQueryResponse::fetch(stream)}};
            case Query::MAGIC:
                return {Variant {Query::fetch(stream)}};
            case QueryPoint::MAGIC:
                return {Variant {QueryPoint::fetch(stream)}};
            case GetQueryResponse::MAGIC:
                return {Variant {GetQueryResponse::fetch(stream)}};
            case GetQueryPointResponse::MAGIC:
                return {Variant {GetQueryPointResponse::fetch(stream)}};
            case GetChunkResponse::MAGIC:
                return {Variant {GetChunkResponse::fetch(stream)}};
            case ReleaseChunkResponse::MAGIC:
                return {Variant {ReleaseChunkResponse::fetch(stream)}};
            case SourceBucket2::MAGIC:
                return {Variant {SourceBucket2::fetch(stream)}};
            default:
                throw TLException(TLException::TYPE::BAD_MAGIC);
        }
    }

    void store(OutputStream& stream) const
    {
        std::visit([&](auto&& data) {
            data.MAGIC.store(stream);
            data.store(stream);
        },
                   m_data);
    }

private:
    HighLevelType(Variant&& data) noexcept
        : m_data(std::move(data))
    {}

    Variant m_data;
    //    AddMetricsBatchQuery::Allocator alloc_1;
    //    GetQueryResponse::Allocator alloc_2;
};

bool operator==(const HighLevelType& lhs, const HighLevelType& rhs) noexcept
{
    return lhs.get() == rhs.get();
}

}    // namespace baseline::statshouse
