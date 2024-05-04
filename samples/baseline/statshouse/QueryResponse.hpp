#pragma once

#include "baseline/statshouse/api/GetQueryResponse.hpp"
#include "baseline/statshouse/public/AddMetricsBatchQuery.hpp"

#include <variant>



namespace baseline::statshouse {

class QueryResponse {
public:
    using Variant = std::variant<AddMetricsBatchQuery,
                                 GetQueryResponse>;

    const Variant& get() const noexcept
    {
        return m_data;
    }

    static QueryResponse fetch(InputStream& stream)
    {
        Magic magic = Magic::fetch(stream);
        switch (magic) {
            case AddMetricsBatchQuery::MAGIC:
                return {Variant {AddMetricsBatchQuery::fetch(stream)}};
            case GetQueryResponse::MAGIC:
                return {Variant {GetQueryResponse::fetch(stream)}};
            default:
                throw TLException(TLException::TYPE::BAD_MAGIC);
        }
    }

    void store(OutputStream& stream)
    {
        std::visit([&](auto&& data) {
            data.store(stream);
        },
                   m_data);
    }

private:
    QueryResponse(Variant&& data) noexcept
        : m_data(std::move(data))
    {}

    Variant m_data;
    //    AddMetricsBatchQuery::Allocator alloc_1;
    //    GetQueryResponse::Allocator alloc_2;
};

}    // namespace baseline::statshouse
