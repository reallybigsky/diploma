#include "test/Environment.hpp"

#include "test/bench/output/AddMetricsBatchQuery.hpp"
#include "test/bench/output/GetChunkResponse.hpp"
#include "test/bench/output/GetQueryPointResponse.hpp"
#include "test/bench/output/GetQueryResponse.hpp"
#include "test/bench/output/HttpQuery.hpp"
#include "test/bench/output/HttpQueryResponse.hpp"
#include "test/bench/output/Query.hpp"
#include "test/bench/output/QueryPoint.hpp"
#include "test/bench/output/ReleaseChunkResponse.hpp"
#include "test/bench/output/SourceBucket2.hpp"

#include <iostream>
#include <iomanip>
#include <unordered_map>



int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cout << "Not enough arguments" << std::endl;
        return 1;
    }

    FILE* file = fopen(argv[1], "wb");
    const size_t seed = std::atoll(argv[2]);
    const size_t file_size = std::atoll(argv[3]) * 1024 * 1024;

    std::unordered_map<std::string, size_t> COUNTERS;
    const size_t BUFFER_SIZE = 256 * 1024 * 1024;

    std::default_random_engine engine(seed);
    std::uniform_int_distribution<size_t> ud(0, 9);

    DataStream::Block block(std::shared_ptr<uint8_t[]>(new uint8_t[BUFFER_SIZE]), BUFFER_SIZE);
    OutputStream os;

    size_t current_file_size = 0;
    while (current_file_size < file_size) {
        os.add(block);

        size_t id = ud(engine);
        switch (id) {
            case 0: {
                statshouse::AddMetricsBatchQuery::Builder data = createAddMetricsBatchQuery(engine);
                ++COUNTERS["AddMetricsBatchQuery"];
                data.store(os);
                break;
            }
            case 1: {
                statshouse::HttpQuery::Builder data = createHttpQuery(engine);
                ++COUNTERS["HttpQuery"];
                data.store(os);
                break;
            }
            case 2: {
                statshouse::HttpQueryResponse::Builder data = createHttpQueryResponse(engine);
                ++COUNTERS["HttpQueryResponse"];
                data.store(os);
                break;
            }
            case 3: {
                statshouse::Query::Builder data = createQuery(engine);
                ++COUNTERS["Query"];
                data.store(os);
                break;
            }
            case 4: {
                statshouse::QueryPoint::Builder data = createQueryPoint(engine);
                ++COUNTERS["QueryPoint"];
                data.store(os);
                break;
            }
            case 5: {
                statshouse::GetQueryResponse::Builder data = createGetQueryResponse(engine);
                ++COUNTERS["GetQueryResponse"];
                data.store(os);
                break;
            }
            case 6: {
                statshouse::GetQueryPointResponse::Builder data = createGetQueryPointResponse(engine);
                ++COUNTERS["GetQueryPointResponse"];
                data.store(os);
                break;
            }
            case 7: {
                statshouse::GetChunkResponse::Builder data = createGetChunkResponse(engine);
                ++COUNTERS["GetChunkResponse"];
                data.store(os);
                break;
            }
            case 8: {
                statshouse::ReleaseChunkResponse::Builder data = createReleaseChunkResponse(engine);
                ++COUNTERS["ReleaseChunkResponse"];
                data.store(os);
                break;
            }
            case 9: {
                statshouse::SourceBucket2::Builder data = createSourceBucket2(engine);
                ++COUNTERS["SourceBucket2"];
                data.store(os);
                break;
            }
            default:
                return 1;
        }
        auto [b, pos] = os.pop();
        current_file_size += pos;
        if (fwrite(b.data(), sizeof(uint8_t), pos, file) != pos)
            return 1;

        std::cout << std::setprecision(3) << (double)current_file_size / double(file_size) * 100 << '%' << std::endl;
    }
    fclose(file);

    std::cout << "STATS:" << std::endl;
    for (const auto& [name, cnt] : COUNTERS) {
        std::cout << name << '\t' << cnt << std::endl;
    }

    return 0;
}