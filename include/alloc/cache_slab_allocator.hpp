#pragma once

/* ------------------------------------------
   SlabCache
   - True Linux-style slab allocator
   - One cache per object type
   - Manages slabs: Empty / Partial / Full
   - Bitmap-based tracking inside each slab
   - Optional constructor / destructor per object
   - All operations are fixed-size and predictable
-------------------------------------------*/

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <vector>
#include <list>
#include <cstdlib>
#include <algorithm>

/* -----------------------------------------------------------
   SLAB STRUCT
   Represents a single slab (a block of memory divided into
   equal-sized object slots).
----------------------------------------------------------- */

struct Slab {
    std::byte* memory;                  // start of slab memory
    std::vector<uint8_t> bitmap;        // 1 = map, 0 = used
    size_t free_count;                   // how many object slots free

    Slab(std::byte* mem, size_t objectCount) 
        : memory(mem), bitmap(objectCount, 1),
          free_count(objectCount) 
    {}
};

/* -----------------------------------------------------------
   SLAB CACHE (the actual allocator)
   Manages: object size, slabs, states, constructors, destructors
----------------------------------------------------------- */

class SlabCache {
    public:
        using Ctor = void(*)(void*);
        using Dtor = void(*)(void*);

        /* ------------------------------------------
        Constructor: Create a slab cache
        object_size : size of each object
        slab_size   : size of each slab (default 4096)
        ctor        : optional constructor per object
        dtor        : optional destructor per object
        -------------------------------------------*/
        SlabCache(size_t object_size,
                size_t slab_size = 4096,
                Ctor ctor = nullptr,
                Dtor dtor = nullptr);
        
        // Returns pointer to one free object slot
        void* allocate();

        // Returns an object back to its slab
        void deallocate(void* ptr);

        // frees all slabs and memory
        ~SlabCache();
    
    private:
        std::size_t object_size_;       // Size of each object
        std::size_t slab_size_;         // Size of each slab (bytes)
        std::size_t objects_per_slab_;  // How many objects fit in the slab

        Ctor ctor_;                     // Optional per-object constructor
        Dtor dtor_;                     // Optional per-object destructor

        std::list<Slab*> empty_slabs_;  // Slabs with all slots free
        std::list<Slab*> partial_slabs_; // Slabs with some free slots
        std::list<Slab*> full_slabs_;   // Slabs with no free slots
        
        /* ------------------------------------------
        create_slab
        - Allocates raw slab memory
        - Initializes bitmap
        -------------------------------------------*/
        Slab* create_slab();

        /* ------------------------------------------
        destroy_slab
        - Runs destructors
        - Frees memory
        -------------------------------------------*/
        void destroy_slab(Slab* slab);

        /* ------------------------------------------
        allocate_from_slab
        - Returns a pointer to first free slot
        -------------------------------------------*/
        void* allocate_from_slab(Slab* slab);

        /* ------------------------------------------
        find_slab_containing
        - Find which slab a pointer belongs to
        -------------------------------------------*/
        Slab* find_slab_containing(void* ptr);

        /* ------------------------------------------
        move_to_empty / move_to_partial
        - Handle slab state transitions
        -------------------------------------------*/
        void move_to_empty(Slab* slab);
        void move_to_partial(Slab* slab);
};