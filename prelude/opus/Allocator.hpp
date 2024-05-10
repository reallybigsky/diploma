#pragma once

#include <cstdint>
#include <cstdlib>


#define DEFAULT_ALLOC

namespace opus {

namespace stats {
size_t total_allocations = 0;
size_t total_bytes_allocated = 0;
}    // namespace stats

class Allocator {
public:
#ifdef DEFAULT_ALLOC
    static void* allocate(size_t size)
    {
        ++stats::total_allocations;
        stats::total_bytes_allocated += size;
        return malloc(size);
    }

    static void deallocate(void* ptr) noexcept
    {
        free(ptr);
    }
#endif
};


}    // namespace opus
