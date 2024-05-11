#include "test/Environment.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sys/stat.h>

#define FLUSH_CACHE

constexpr size_t MAX_MEMORY_SIZE = 50 * 1024 * 1024;
constexpr size_t CONSUME_ITERATIONS = 1;
constexpr size_t CPU_CACHE_SIZE = 12 * 1024 * 1024;

size_t flush_cache()
{
    size_t result = 0;
    uint8_t* ptr = new uint8_t[CPU_CACHE_SIZE];
    for (size_t i = 0; i < CPU_CACHE_SIZE; ++i) {
        ptr[i] = rand();
    }

    for (size_t i = 0; i < CPU_CACHE_SIZE; ++i) {
        result += ptr[i];
    }

    delete[] ptr;
    return result;
}



int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Not enough arguments" << std::endl;
        return 1;
    }

    FILE* file = fopen(argv[1], "rb");
    const size_t BLOCK_SIZE = std::atoll(argv[2]);

    struct stat stat_buf;
    int rc = fstat(fileno(file), &stat_buf);
    if (rc)
        return 1;

    const size_t file_size = stat_buf.st_size;
    size_t curr_file_pos = 0;

    InputStream is;
    std::list<DataStream::Block> storage;

    size_t cache_rand = 0;
    size_t result = 0;
    size_t total_microseconds = 0;
    size_t fetch_microseconds = 0;
    size_t consume_microseconds = 0;
    while (curr_file_pos < file_size || is.remainingBytes() > 0) {
        Allocator::startScope();
        while (is.remainingBytes() < MAX_MEMORY_SIZE && curr_file_pos < file_size) {
            const size_t curr_block_size = std::min(file_size - curr_file_pos, BLOCK_SIZE);
            DataStream::Block block(std::shared_ptr<uint8_t[]>(new uint8_t[curr_block_size]), curr_block_size);
            size_t bytes_read = fread(block.data(), sizeof(uint8_t), curr_block_size, file);
            is.add(block);
            storage.push_back(block);
            curr_file_pos += bytes_read;
        }

        {
            std::chrono::high_resolution_clock::time_point fetch_start = std::chrono::high_resolution_clock::now();
            statshouse::HighLevelType data = statshouse::HighLevelType::fetch(is);
            std::chrono::high_resolution_clock::time_point fetch_end = std::chrono::high_resolution_clock::now();

            total_microseconds += std::chrono::duration_cast<std::chrono::microseconds>(fetch_end - fetch_start).count();
            fetch_microseconds += std::chrono::duration_cast<std::chrono::microseconds>(fetch_end - fetch_start).count();

            std::visit([&](auto&& arg) {
                for (size_t i = 0; i < CONSUME_ITERATIONS; ++i) {
                    //#ifdef FLUSH_CACHE
                    //                cache_rand += flush_cache();
                    //#endif
                    std::chrono::high_resolution_clock::time_point consume_start = std::chrono::high_resolution_clock::now();
                    result += consume(arg);
                    std::chrono::high_resolution_clock::time_point consume_end = std::chrono::high_resolution_clock::now();

                    total_microseconds += std::chrono::duration_cast<std::chrono::microseconds>(consume_end - consume_start).count();
                    consume_microseconds += std::chrono::duration_cast<std::chrono::microseconds>(consume_end - consume_start).count();
                }
            },
                       data.get());
        }

        while (!storage.empty() && storage.front().sptr.use_count() == 1) {
            storage.pop_front();
        }

        std::cout << std::setprecision(3) << (double)curr_file_pos / double(file_size) * 100 << '%' << std::endl;
//        std::cout << result << std::endl;
        Allocator::endScope();
    }

    std::cout << "CACHE_RAND: " << cache_rand << std::endl;
    std::cout << "CONSUME_RESULT: " << result << std::endl;
    std::cout << std::endl;
    std::cout << "TOTAL_TIME_MICROSECONDS: " << total_microseconds << std::endl;
    std::cout << "FETCH_TIME_MICROSECONDS: " << fetch_microseconds << std::endl;
    std::cout << "CONSUME_TIME_MICROSECONDS: " << consume_microseconds << std::endl;
    std::cout << std::endl;
    std::cout << "TOTAL_ALLOCATIONS: " << stats::total_allocations << std::endl;
    std::cout << "TOTAL_BYTES_ALLOCATED: " << stats::total_bytes_allocated << std::endl;

    fclose(file);
    return 0;
}