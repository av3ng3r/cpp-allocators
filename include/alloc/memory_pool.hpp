#pragma once

#include <cstddef>
#include <vector>
#include <cassert>

//
// Fixed Memory Pool Allocator
// - Allocates fixed-size blocks
// - O(1) alloc/dealloc
// - Chunk-based automatic expansion
// - Suitable for small, uniform objects
//

class MemoryPool {
public:
    MemoryPool(std::size_t block_size, std::size_t blocks_per_chunk);

    void* allocate();
    void  deallocate(void* ptr);

    ~MemoryPool();

private:
    struct FreeNode { FreeNode* next; };

    std::size_t block_size_;
    std::size_t blocks_per_chunk_;

    FreeNode* free_list_ = nullptr;
    std::vector<void*> chunks_;

    static std::size_t aligned_block_size(std::size_t size);
    void add_chunk();
};
