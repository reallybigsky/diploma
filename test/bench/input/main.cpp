#include "test/Environment.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>



constexpr size_t MAX_MEMORY_SIZE = 32 * 1024 * 1024;
constexpr size_t WARMUP_ITERATIONS = 5;
constexpr size_t WARMUP_BLOCK_SIZE = 1024 * 1024;
constexpr size_t BENCH_ITERATIONS = 10;
constexpr size_t CONSUME_ITERATIONS = 1;
const std::vector<size_t> BLOCK_SIZES = {
        1024,
        2048,
        4096,
        8192,
        16384,
        32768,
        65536,
        131072,
        262144,
        524288,
        1048576,
        2097152,
        4194304,
        8388608,
        16777216
};



void RunBenchmarkIter(const std::filesystem::path& input_file_path,
                      const std::filesystem::path& output_dir_path,
                      size_t block_size,
                      bool is_warmup)
{
    std::ifstream input_file(input_file_path, std::ios::binary | std::ios::in);

    size_t input_file_size = std::filesystem::file_size(input_file_path);
    size_t curr_file_pos = 0;

    InputStream is;
    std::list<DataStream::Block> storage;

    stats::total_allocations = 0;
    stats::total_bytes_allocated = 0;

    size_t result = 0;
    size_t total_microseconds = 0;
    size_t fetch_microseconds = 0;
    size_t consume_microseconds = 0;
    while (curr_file_pos < input_file_size || is.remainingBytes() > 0) {
        while (is.remainingBytes() < MAX_MEMORY_SIZE && curr_file_pos < input_file_size) {
            const size_t curr_block_size = std::min(input_file_size - curr_file_pos, block_size);
            DataStream::Block block(std::shared_ptr<uint8_t[]>(new uint8_t[curr_block_size]), curr_block_size);
            input_file.read((char*)block.data(), curr_block_size);
            is.add(block);
            storage.push_back(block);
            curr_file_pos += curr_block_size;
        }

        std::chrono::high_resolution_clock::time_point fetch_start = std::chrono::high_resolution_clock::now();
        Allocator::startScope();
        {
            statshouse::HighLevelType data = statshouse::HighLevelType::fetch(is);
            fetch_microseconds += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - fetch_start).count();

            std::visit([&](auto&& arg) {
                for (size_t i = 0; i < CONSUME_ITERATIONS; ++i) {
                    std::chrono::high_resolution_clock::time_point consume_start = std::chrono::high_resolution_clock::now();
                    result += consume(arg);
                    consume_microseconds += std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::high_resolution_clock::now() - consume_start).count();
                }
            },
                       data.get());
        }
        Allocator::endScope();
        total_microseconds += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - fetch_start).count();

        while (!storage.empty() && storage.front().sptr.use_count() == 1) {
            storage.pop_front();
        }
        //        std::cout << std::setprecision(3) << (double)curr_file_pos / double(file_size) * 100 << '%' << std::endl;
    }

    if (!is_warmup) {
        std::ofstream output_file(output_dir_path / (std::to_string(block_size) + ".txt"), std::ios::app | std::ios::out);
        output_file << total_microseconds << ' '
                    << fetch_microseconds << ' '
                    << consume_microseconds << ' '
                    << stats::total_allocations << ' '
                    << stats::total_bytes_allocated << ' ' << std::endl;

        std::cout << "CONSUME_RESULT: " << result << std::endl;
        std::cout << std::endl;
        std::cout << "TOTAL_TIME_MICROSECONDS: " << total_microseconds << std::endl;
        std::cout << "FETCH_TIME_MICROSECONDS: " << fetch_microseconds << std::endl;
        std::cout << "CONSUME_TIME_MICROSECONDS: " << consume_microseconds << std::endl;
        std::cout << std::endl;
        std::cout << "TOTAL_ALLOCATIONS: " << stats::total_allocations << std::endl;
        std::cout << "TOTAL_BYTES_ALLOCATED: " << stats::total_bytes_allocated << std::endl;
    }
}



int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Not enough arguments" << std::endl;
        return 1;
    }

    std::filesystem::path input_file_path(argv[1]);
    std::filesystem::path output_dir_path(argv[2]);

    std::cout << "WARMUP START" << std::endl;
    for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
        RunBenchmarkIter(input_file_path, output_dir_path, WARMUP_BLOCK_SIZE, true);
    }
    std::cout << "WARMUP END" << std::endl;

    for (size_t block_size : BLOCK_SIZES) {
        for (size_t i = 0; i < BENCH_ITERATIONS; ++i) {
            std::cout << std::endl
                      << block_size << ' ' << i + 1 << '/' << BENCH_ITERATIONS << std::endl;
            RunBenchmarkIter(input_file_path, output_dir_path, block_size, false);
        }
    }

    return 0;
}