#include "alloc/arena_allocator.hpp"

ArenaAllocator::ArenaAllocator(std::size_t size) : 
                        size_(size),
                        start_(static_cast<std::byte*>(::operator new (size))),
                        current_(start_),
                        end_(start_ + size) {}

std::byte* ArenaAllocator::align_ptr(std::byte* ptr, std::size_t alignment) noexcept {
    std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(ptr);
    std::size_t misalignment = addr % alignment;

    if(misalignment == 0) return ptr;

    return ptr + (alignment - misalignment);
}

void* ArenaAllocator::allocate(std::size_t n, std::size_t alignment) {
    std::byte* aligned = align_ptr(current_, alignment);

    if(aligned + n > end_) return nullptr;  // Out of memory

    current_ = aligned + n;
    return aligned;
}

void ArenaAllocator::reset() noexcept {
    current_ = start_;
}

std::size_t ArenaAllocator::remaining() const noexcept {
    return static_cast<std::size_t>(end_ - current_);
}

std::size_t ArenaAllocator::size() const noexcept {
    return size_;
}

ArenaAllocator::~ArenaAllocator()
{
    ::operator delete(start_);
}