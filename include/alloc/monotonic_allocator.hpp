#pragma once

//
// Monotonic Allocator
// - Linear bump-pointer allocation
// - Never frees individual objects (reset-all model)
// - Automatically grows by allocating new blocks
// - Zero fragmentation, excellent locality
// - Ideal for short-lived, bursty allocations
//

#include <cstddef>
#include <vector>
#include <new>
#include <memory>

class MonotonicAllocator {
    public:
        explicit MonotonicAllocator(std::size_t initial_block = 1024);
        void* allocate(std::size_t size, std::size_t alignement = alignof(std::max_align_t));
        void reset() noexcept;
        std::size_t remaining_in_current_block() const;
        ~MonotonicAllocator() = default;

    private:
        std::byte* start_ = nullptr;
        std::byte* current_ = nullptr;
        std::byte* end_ = nullptr;

        std::size_t initial_block_size_;

        std::vector<std::unique_ptr<std::byte[]>> blocks_;
        std::vector<std::size_t> block_sizes_;

        void add_block(std::size_t size);
        static std::byte* align_ptr(std::byte* p, std::size_t alignment);
        void grow(std::size_t required);
};