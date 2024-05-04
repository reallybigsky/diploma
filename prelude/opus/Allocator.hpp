#pragma once

#include <cstdint>
#include <cstdlib>


#define DEFAULT_ALLOC

namespace opus::alloc {

#ifdef DEFAULT_ALLOC
void* allocate(size_t size)
{
    return malloc(size);
}

void deallocate(void* ptr) noexcept
{
    free(ptr);
}
#endif

}    // namespace alloc
