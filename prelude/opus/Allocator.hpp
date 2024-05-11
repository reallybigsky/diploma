#pragma once

#include "lib/MemoryPool/MemoryPool.h"

#include <cstdint>
#include <cstdlib>


//#define DEFAULT_ALLOC

namespace opus {

namespace stats {
size_t total_allocations = 0;
size_t total_bytes_allocated = 0;
}    // namespace stats

//#ifdef DEFAULT_ALLOC
//class Allocator {
//public:
//    static void* allocate(size_t size)
//    {
//        ++stats::total_allocations;
//        stats::total_bytes_allocated += size;
//        return malloc(size);
//    }
//
//    static void deallocate(void* ptr) noexcept
//    {
//        free(ptr);
//    }
//
//    static void startScope() {}
//
//    static void endScope() {}
//};
//#elif
class Allocator {
public:
    static constexpr size_t MEMPOOL_BLOCK_SIZE = 2 * 1024 * 1024;

    static void* allocate(size_t size)
    {
        return m_pool.allocate(size);
    }

    static void deallocate(void*) noexcept
    {}

    static void startScope()
    {
        m_pool.startScope();
    }

    static void endScope()
    {
        m_pool.endScope();
    }

private:
    inline static AppShift::Memory::MemoryPool m_pool = AppShift::Memory::MemoryPool(MEMPOOL_BLOCK_SIZE);
};

//#endif

}    // namespace opus
