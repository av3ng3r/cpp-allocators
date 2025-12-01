#include "alloc/cache_slab_allocator.hpp"
#include <iostream>

struct User
{
    int id;
    std::string name;
};

// Constructor callback
void user_ctor(void* p)
{
    new (p) User{0, ""}; // placement new
}

// Destructor callback
void user_dtor(void* p)
{
    static_cast<User*>(p)->~User();
}

int main()
{
    SlabCache cache(sizeof(User), 4096, user_ctor, user_dtor);

    User* u1 = static_cast<User*>(cache.allocate());
    u1->id = 42;
    u1->name = "Neo";

    std::cout << "User: " << u1->id << ", " << u1->name << "\n";

    cache.deallocate(u1);

    return 0;
}
