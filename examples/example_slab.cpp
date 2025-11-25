#include "alloc/slab_allocator.hpp"
#include <iostream>

struct EventSmall {
    int a, b;
};

struct EventLarge {
    char buffer[700];  // 700 bytes → 1024-byte class
};

int main() {
    SlabAllocator slab(512);

    // Small event
    void* es_mem = slab.allocate(sizeof(EventSmall));
    auto* es = new (es_mem) EventSmall{1, 2};

    // Large event
    void* el_mem = slab.allocate(sizeof(EventLarge));
    auto* el = new (el_mem) EventLarge{};

    std::cout << "Small event: a=" << es->a << " b=" << es->b << "\n";
    std::cout << "Large event created (700 bytes)\n";

    es->~EventSmall();
    slab.deallocate(es, sizeof(EventSmall));

    el->~EventLarge();
    slab.deallocate(el, sizeof(EventLarge));
}
