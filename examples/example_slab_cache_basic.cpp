#include "alloc/cache_slab_allocator.hpp"
#include <iostream>

struct Order
{
    int id;
    double price;
};

int main()
{
    // Each Order is sizeof(Order)
    SlabCache cache(sizeof(Order));

    // Allocate one Order object
    void* ptr = cache.allocate();
    auto* order = static_cast<Order*>(ptr);

    // Use the object
    order->id = 101;
    order->price = 249.99;

    std::cout << "Order: " << order->id 
              << " Price: " << order->price << "\n";

    // Free the object back to the slab
    cache.deallocate(order);

    return 0;
}