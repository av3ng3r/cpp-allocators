#pragma once

//
// Arena Allocator
// - Fastest possible allocator (bump-pointer)
// - Single contiguous memory region
// - O(1) allocation: pointer increment
// - No per-object free; memory reclaimed via reset()
// - Zero fragmentation, strong locality
//

#include <cstddef>
#include <cassert>
#include <new>
#include <memory>
#include <algorithm>

class ArenaAllocator
{
private:
    std::size_t size_;      // Total size of the arena
    std::byte* start_;      // Start of the memory block
    std::byte* current_;    // Bump pointer (next allocation point)
    std::byte* end_;        // End of the memory block (start_ + size_)

    // Align pointer forward to the required boundary
    static std::byte* align_ptr(std::byte* ptr, std::size_t alignment) noexcept;

public:
    // Allocate a contiguous block of memory of given size
    // Alignment defaults to max_align_t for general-purpose use
    explicit ArenaAllocator(std::size_t size);

    // Fast O(1) bump allocation; returns nullptr if arena is full
    void* allocate(std::size_t n, std::size_t alignment = alignof(std::max_align_t));

    // Reset arena: all allocations become invalid, bump pointer returns to start
    void reset() noexcept;

    // Remaining free bytes in the arena
    std::size_t remaining() const noexcept;

    // Total capacity of the arena
    std::size_t size() const noexcept;

    // Release the backing memory
    ~ArenaAllocator();
};
