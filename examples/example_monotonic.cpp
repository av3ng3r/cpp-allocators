#include "alloc/monotonic_allocator.hpp"
#include <iostream>
#include <cstring>

// A simple struct we want to allocate many times
struct Item {
    int id;
    double value;
};

int main() {
    // Start with a small initial block (to force multiple grows)
    MonotonicAllocator alloc(128);

    std::cout << "Initial block allocated.\n";

    for (int batch = 1; batch <= 3; ++batch) {
        std::cout << "\n=== Batch " << batch << " ===\n";

        // Allocate 20 items
        for (int i = 0; i < 20; ++i) {
            Item* it = static_cast<Item*>(alloc.allocate(sizeof(Item), alignof(Item)));

            it->id = i;
            it->value = i * 1.5;

            // Print one or two just to show it's working
            if (i < 2)
                std::cout << "Allocated Item { id=" << it->id 
                          << ", value=" << it->value << " }\n";
        }

        // Show remaining space in current block (for debugging)
        std::cout << "Remaining in current block: " 
                  << alloc.remaining_in_current_block() << " bytes\n";

        // Reset everything at once
        std::cout << "Resetting allocator...\n";
        alloc.reset();
    }

    return 0;
}
