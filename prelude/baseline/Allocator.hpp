#pragma once

#include <memory>
#include <vector>



namespace baseline {

namespace stats {
size_t total_allocations = 0;
size_t total_bytes_allocated = 0;
}    // namespace stats

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

template <typename T>
using VectorImpl = std::vector<T, VectorAllocator<T>>;

}    // namespace baseline
