#pragma once

//
// Stack Allocator
// - LIFO allocation model (push/pop markers)
// - Linear bump-pointer allocation
// - O(1) allocate and O(1) pop()
// - Zero fragmentation
// - Ideal for nested, scoped temporary allocations
//

#include <cstddef>
#include <new>
#include <memory>

class StackAllocator {
    public:
        using Marker = std::byte*;
        explicit StackAllocator(std::size_t size);
        void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t));
        Marker push() const noexcept;
        void pop(Marker marker) noexcept;
        std::size_t size() const noexcept;
        std::size_t remaining() const noexcept;
        ~StackAllocator();

    private:
        std::size_t size_;
        std::byte* start_;
        std::byte* current_;
        std::byte* end_;

        static std::byte* align_ptr(std::byte* p, std::size_t alignment) noexcept;
};