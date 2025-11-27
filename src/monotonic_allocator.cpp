#include "alloc/monotonic_allocator.hpp"

MonotonicAllocator::MonotonicAllocator(std::size_t initial_block_size)
        :initial_block_size_(initial_block_size)
    {
        add_block(initial_block_size_);
    }

void MonotonicAllocator::add_block(std::size_t size) {
    blocks_.push_back(std::make_unique<std::byte[]>(size));
    block_sizes_.push_back(size);

    start_ = blocks_.back().get();
    current_ = start_;
    end_ = start_ + size;
}

void* MonotonicAllocator::allocate(std::size_t n, std::size_t alignment) {
    std::byte* aligned = align_ptr(current_, alignment);

    if(aligned + n > end_) {
        grow(n + alignment);
        aligned = align_ptr(current_, alignment);
    }

    current_ = aligned + n;
    return aligned;
}

void MonotonicAllocator::reset() noexcept {
    block_sizes_.resize(1);

    start_ = blocks_[0].get();
    current_ = start_;
    end_ = start_ + block_sizes_[0];
}

std::size_t MonotonicAllocator::remaining_in_current_block() const {
    return static_cast<std::size_t>(end_ - current_);
}

void MonotonicAllocator::grow(std::size_t required) {
    std::size_t new_size = std::max(required, block_sizes_.back()*2);

    add_block(new_size);
}

std::byte* MonotonicAllocator::align_ptr(std::byte* p, std::size_t alignment) {
    std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(p);
    std::size_t misalignment = addr % alignment;

    if(misalignment == 0) return p;

    return p + (alignment - misalignment);
}