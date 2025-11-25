#include "alloc/memory_pool.hpp"
#include <iostream>

struct Order {
    int id;
    double price;
    int qty;

    Order(int i, double p, int q)
        : id(i), price(p), qty(q) {}
};

int main() {
    // A pool for Order objects
    MemoryPool pool(sizeof(Order), 1024);

    // Allocate raw memory and construct object
    void* mem1 = pool.allocate();
    Order* o1 = new (mem1) Order(101, 89.5, 3);

    std::cout << "Order " << o1->id
              << " price=" << o1->price
              << " qty=" << o1->qty << "\n";

    // Destroy and free
    o1->~Order();
    pool.deallocate(o1);

    // Allocate again (likely returns the same block)
    void* mem2 = pool.allocate();
    Order* o2 = new (mem2) Order(102, 90.1, 5);

    std::cout << "Order " << o2->id << " created.\n";

    o2->~Order();
    pool.deallocate(o2);
}
