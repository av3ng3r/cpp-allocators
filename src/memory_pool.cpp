#include "alloc/memory_pool.hpp"
#include <new>
#include <algorithm>

MemoryPool::MemoryPool(std::size_t block_size, std::size_t blocks_per_chunk)
    : block_size_(aligned_block_size(block_size)),
      blocks_per_chunk_(blocks_per_chunk)
{
    assert(block_size > 0);
    assert(blocks_per_chunk > 0);
    add_chunk();
}

std::size_t MemoryPool::aligned_block_size(std::size_t size) {
    std::size_t min = std::max(size, sizeof(FreeNode));
    std::size_t alignment = alignof(std::max_align_t);
    return (min + alignment - 1) & ~(alignment - 1);
}

void MemoryPool::add_chunk() {
    std::size_t chunk_size = block_size_ * blocks_per_chunk_;
    void* chunk = ::operator new(chunk_size);

    chunks_.push_back(chunk);

    char* p = static_cast<char*>(chunk);
    for (std::size_t i = 0; i < blocks_per_chunk_; ++i) {
        FreeNode* node = reinterpret_cast<FreeNode*>(p);
        node->next = free_list_;
        free_list_ = node;
        p += block_size_;
    }
}

void* MemoryPool::allocate() {
    if (!free_list_) {
        add_chunk();
    }

    FreeNode* node = free_list_;
    free_list_ = node->next;
    return node;
}

void MemoryPool::deallocate(void* ptr) {
    assert(ptr != nullptr);

    FreeNode* node = static_cast<FreeNode*>(ptr);
    node->next = free_list_;
    free_list_ = node;
}

MemoryPool::~MemoryPool() {
    for (void* chunk : chunks_) {
        ::operator delete(chunk);
    }
}
