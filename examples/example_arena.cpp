#include "alloc/arena_allocator.hpp"
#include <iostream>

// A small struct to test aligned allocations
struct Foo {
    int x;
    double y;
};

int main() {
    ArenaAllocator arena(1024);

    std::cout << "Arena Size: " << arena.size() << " bytes\n";

    int* a = static_cast<int*>(arena.allocate(sizeof(int), alignof(int)));
    *a = 42;
    std::cout << "Allocate int: " << *a << "\n";
    std::cout << "Remaining: " << arena.remaining() << " bytes\n";

    double* b = static_cast<double*>(arena.allocate(sizeof(double), alignof(double)));
    *b = 4.12345;
    std::cout << "Allocateed double: " << *b << "\n";
    std::cout << "Remaining: " << arena.remaining() << " bytes\n";

    
    // Allocate a struct Foo
    Foo* f = static_cast<Foo*>(arena.allocate(sizeof(Foo), alignof(Foo)));
    f->x = 7;
    f->y = 2.71828;

    std::cout << "Allocated Foo: x=" << f->x << " y=" << f->y << "\n";
    std::cout << "Remaining: " << arena.remaining() << " bytes\n";

    // Allocate a buffer of 100 bytes
    void* buf = arena.allocate(100);
    if (buf)
        std::cout << "Allocated 100-byte buffer.\n";
    else
        std::cout << "Buffer allocation failed.\n";

    std::cout << "Remaining: " << arena.remaining() << " bytes\n";

    // Reset the arena (frees everything)
    std::cout << "\nResetting arena...\n";
    arena.reset();

    std::cout << "Remaining after reset: " << arena.remaining() 
              << " bytes (should equal total size)\n";

    // Allocate again after reset
    int* afterReset = static_cast<int*>(arena.allocate(sizeof(int)));
    *afterReset = 99;
    std::cout << "Allocated int after reset: " << *afterReset << "\n";

    std::cout << "Remaining: " << arena.remaining() << " bytes\n";
}