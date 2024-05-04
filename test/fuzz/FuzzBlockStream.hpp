#include "fuzztest/fuzztest.h"
#include <gtest/gtest.h>

#include "test/Utils.hpp"



class FuzzBlockStream {
public:
    static constexpr size_t MIN_MEMORY_SIZE = 5 * 1024 * 1024;
    static constexpr size_t MAX_MEMORY_SIZE = 10 * 1024 * 1024;

    FuzzBlockStream()
    {
        checkBlockStreamSize();
    }


protected:
    static constexpr size_t BLOCKS[] = {
            16,
            32,
            64,
            128,
            256,
            512,
            1024,
            2048,
            4096};

    void checkBlockStreamSize()
    {
        static std::random_device rd;
        static std::uniform_int_distribution<int> dist(0, (sizeof(BLOCKS) / sizeof(decltype(BLOCKS[0]))) - 1);

        ASSERT_EQ(os.pos(), is.pos());

        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
        if (os.remainingBytes() < MIN_MEMORY_SIZE) {
            while (!storage.empty() && storage.front().sptr.use_count() == 1) {
                storage.pop_front();
            }

            while (os.remainingBytes() < MAX_MEMORY_SIZE) {
                auto block = createBlock(BLOCKS[dist(rd)]);
                os.add(block);
                is.add(block);
                storage.push_back(block);
            }
        }

        ASSERT_EQ(os.pos(), is.pos());
        ASSERT_EQ(os.remainingBytes(), is.remainingBytes());
    }

protected:
    InputStream is;
    OutputStream os;
    std::list<DataStream::Block> storage;
};
