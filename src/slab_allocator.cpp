#include "alloc/slab_allocator.hpp"
#include <cassert>

SlabAllocator::SlabAllocator(std::size_t blocks_per_chunk)
    : blocks_per_chunk_(blocks_per_chunk)
{
    pools_.fill(nullptr);
}

std::size_t SlabAllocator::class_index(std::size_t size) const {
    for (std::size_t i = 0; i < NUM_CLASSES; ++i) {
        if (size <= size_classes_[i])
            return i;
    }
    return NUM_CLASSES; // too large
}

void* SlabAllocator::allocate(std::size_t size) {
    std::size_t idx = class_index(size);
    if (idx == NUM_CLASSES) {
        return nullptr; // unsupported size
    }

    if (!pools_[idx]) {
        pools_[idx] = new MemoryPool(size_classes_[idx], blocks_per_chunk_);
    }

    return pools_[idx]->allocate();
}

void SlabAllocator::deallocate(void* ptr, std::size_t size) {
    assert(ptr != nullptr);
    std::size_t idx = class_index(size);
    assert(idx != NUM_CLASSES);
    pools_[idx]->deallocate(ptr);
}

SlabAllocator::~SlabAllocator() {
    for (MemoryPool* pool : pools_) {
        delete pool;
    }
}
