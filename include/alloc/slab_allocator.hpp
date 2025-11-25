#pragma once

#include <cstddef>
#include <array>
#include "alloc/memory_pool.hpp"

//
// Slab Allocator
// - Multiple fixed memory pools for variable small sizes
// - Power-of-two size classes: 8 → 4096
// - Lazy pool initialization
//

class SlabAllocator {
public:
    explicit SlabAllocator(std::size_t blocks_per_chunk = 1024);

    void* allocate(std::size_t size);
    void  deallocate(void* ptr, std::size_t size);

    ~SlabAllocator();

private:
    static constexpr std::size_t size_classes_[10] = {
        8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096
    };

    static constexpr std::size_t NUM_CLASSES = 10;

    std::size_t blocks_per_chunk_;
    std::array<MemoryPool*, NUM_CLASSES> pools_{};

    std::size_t class_index(std::size_t size) const;
};
