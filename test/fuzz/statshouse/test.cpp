#include "fuzztest/fuzztest.h"
#include <gtest/gtest.h>

#include "fuzz/FuzzBlockStream.hpp"

#include "fuzz/statshouse/api/GetQueryResponse.hpp"
#include "fuzz/statshouse/public/AddMetricsBatchQuery.hpp"

#include "baseline/statshouse/QueryResponse.hpp"



auto StatshouseDomain()
{
    return fuzztest::VariantOf(fuzz_details::domainAddMetricsBatchQuery());
};

using QueryResponseVariant = std::variant<fuzz_details::AddMetricsBatchQuery>;


class StatshouseFuzzBlockStream : public FuzzBlockStream {
public:
    StatshouseFuzzBlockStream()
        : FuzzBlockStream()
    {
    }

    ~StatshouseFuzzBlockStream()
    {}

    void StoreFetch(const QueryResponseVariant& input)
    {
        checkBlockStreamSize();
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            typename T::Builder builder = arg.toBuilder();
            builder.store(os);

            ::statshouse::QueryResponse result = ::statshouse::QueryResponse::fetch(is);
            std::visit([&](auto&& inner) {
                //                ASSERT_EQ(inner, builder);
                if (inner != builder) {
                    std::cout << "TUT" << std::endl;
                }

                testCopyMove(inner);
            },
                       result.get());
        },
                   input);
    }
};



FUZZ_TEST_F(StatshouseFuzzBlockStream, StoreFetch).WithDomains(StatshouseDomain());
