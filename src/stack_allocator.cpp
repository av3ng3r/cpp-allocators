#include "alloc/stack_allocator.hpp"

StackAllocator::StackAllocator(std::size_t size) 
    :   size_(size), 
        start_(static_cast<std::byte*>(::operator new(size))),
        current_(start_),
        end_(start_ + size) 
{}

void* StackAllocator::allocate(std::size_t n, std::size_t alignment) {
    std::byte* aligned = align_ptr(current_, alignment);

    if(aligned + n > end_) {
        return nullptr; // Out of memory
    }

    current_ = aligned + n;
    return aligned;
}

StackAllocator::Marker StackAllocator::push() const noexcept {
    return current_;
}

void StackAllocator::pop(StackAllocator::Marker marker) noexcept {
    if(marker >= start_ && marker <= end_){
        current_ = marker;
    }
}

std::size_t StackAllocator::size() const noexcept {
    return size_;
}

std::size_t StackAllocator::remaining() const noexcept {
    return static_cast<size_t>(end_ - current_);
}

std::byte* StackAllocator::align_ptr(std::byte* p, std::size_t alignment) noexcept {
    std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(p);
    std::size_t misalignment = addr % alignment;

    if(alignment == 0) return p;

    return p + (alignment - misalignment);
}

StackAllocator::~StackAllocator() {
    ::operator delete(start_);
}
