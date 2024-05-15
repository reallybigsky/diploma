#pragma once

#include "lib/MemoryPool/MemoryPool.h"

#include <memory>
#include <vector>

#define BASELINE_STD_ALLOC
//#define BASELINE_POOL_ALLOC

namespace stats {
extern size_t total_allocations;
extern size_t total_bytes_allocated;
}    // namespace stats

namespace baseline {

#ifdef BASELINE_STD_ALLOC
class Allocator {
public:
    static void startScope() {}

    static void endScope() {}
};

template <typename T>
class VectorAllocator {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;

    VectorAllocator() noexcept = default;

    VectorAllocator(const VectorAllocator&) noexcept = default;
    VectorAllocator(VectorAllocator&&) noexcept = default;
    VectorAllocator& operator=(const VectorAllocator&) noexcept = default;
    VectorAllocator& operator=(VectorAllocator&&) noexcept = default;

    ~VectorAllocator() noexcept = default;

    [[nodiscard]] constexpr T* allocate(std::size_t n)
    {
        stats::total_allocations += 1;
        stats::total_bytes_allocated += n * sizeof(T);
        return impl.allocate(n);
    }

    void deallocate(T* p, std::size_t n)
    {
        impl.deallocate(p, n);
    }

private:
    std::allocator<T> impl;
};
#elifdef BASELINE_POOL_ALLOC
class Allocator {
public:
    static constexpr size_t MEMPOOL_BLOCK_SIZE = 64 * 1024 * 1024;

    static void* allocate(size_t size)
    {
        return m_pool.allocate(size);
    }

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

template <typename T>
class VectorAllocator {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;

    VectorAllocator() noexcept = default;

    VectorAllocator(const VectorAllocator&) noexcept = default;
    VectorAllocator(VectorAllocator&&) noexcept = default;
    VectorAllocator& operator=(const VectorAllocator&) noexcept = default;
    VectorAllocator& operator=(VectorAllocator&&) noexcept = default;

    ~VectorAllocator() noexcept = default;

    [[nodiscard]] constexpr T* allocate(std::size_t n)
    {
        return (T*)Allocator::allocate(n * sizeof(T));
    }

    void deallocate(T*, std::size_t)
    {}

    static void startScope()
    {
        Allocator::startScope();
    }

    static void endScope()
    {
        Allocator::endScope();
    }

private:
};

#endif

template <typename T>
using VectorImpl = std::vector<T, VectorAllocator<T>>;

}    // namespace baseline
