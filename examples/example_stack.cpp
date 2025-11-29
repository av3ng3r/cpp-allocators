#include <iostream>
#include "alloc/stack_allocator.hpp"

struct Node {
    int a;
    float b;
};

void compute_inner(StackAllocator& alloc, int depth) {
    std::cout << "  Entering inner level " << depth << "\n";

    // Save marker for this nested scope
    auto m = alloc.push();

    // Allocate some temporary objects
    for (int i = 0; i < 5; ++i) {
        Node* n = static_cast<Node*>(alloc.allocate(sizeof(Node), alignof(Node)));
        n->a = depth * 10 + i;
        n->b = n->a * 0.5f;

        if (i < 2) {
            std::cout << "    Allocated Node{ a=" << n->a 
                      << ", b=" << n->b << " }\n";
        }
    }

    std::cout << "  Used some memory. Remaining: " 
              << alloc.remaining() << " bytes\n";

    // End of this scope — free all objects allocated above marker
    alloc.pop(m);
    std::cout << "  Exiting level " << depth 
              << ", memory restored. Remaining: " 
              << alloc.remaining() << " bytes\n";
}

int main() {
    StackAllocator alloc(1024); // 1 KB stack allocator

    std::cout << "Initial remaining: " << alloc.remaining() << " bytes\n\n";

    // Outer scope
    auto outer = alloc.push();

    // Allocate something in outer scope
    Node* root = static_cast<Node*>(alloc.allocate(sizeof(Node)));
    root->a = 1;
    root->b = 0.5f;

    std::cout << "Allocated root node. Remaining: " 
              << alloc.remaining() << " bytes\n\n";

    // Nested scopes
    compute_inner(alloc, 1);
    compute_inner(alloc, 2);
    compute_inner(alloc, 3);

    // Cleanup outer scope
    alloc.pop(outer);

    std::cout << "\nAfter popping outer scope. Remaining: " 
              << alloc.remaining() << " bytes\n";

    return 0;
}
