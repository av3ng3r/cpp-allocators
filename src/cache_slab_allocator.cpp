#include "alloc/cache_slab_allocator.hpp"

SlabCache::SlabCache(std::size_t object_size,
                    std::size_t slab_size,
                    Ctor ctor,
                    Dtor dtor)
    : object_size_(object_size),
    slab_size_(slab_size),
    ctor_(ctor),
    dtor_(dtor)
{
    objects_per_slab_ = slab_size_ / object_size_;
    assert(objects_per_slab_ > 0 && "slab_size too small for object_size");
}

SlabCache::~SlabCache()
{
    for(auto* s : empty_slabs_)     destroy_slab(s);
    for(auto* s : partial_slabs_)    destroy_slab(s);
    for(auto* s : full_slabs_)      destroy_slab(s);
}

Slab* SlabCache::create_slab()
{
    std::byte* mem = static_cast<std::byte*>(std::malloc(slab_size_));

    assert(mem && "malloc failed for slab");

    Slab* slab = new Slab(mem, objects_per_slab_);

    if(ctor_)
    {
        for(std::size_t i = 0; i < objects_per_slab_; ++i) {
            std::byte* slot = mem + i * object_size_;
            ctor_(static_cast<void*>(slot));
        }
    }

    empty_slabs_.push_back(slab);
    return slab;
}

void SlabCache::destroy_slab(Slab* slab)
{
    if(dtor_)
    {
        for (std::size_t i = 0; i < objects_per_slab_; ++i) {
            if(slab->bitmap[i] == 0)
            {
                std::byte* slot = slab->memory + i * object_size_;
                dtor_(static_cast<void*>(slot));
            }
        }
    }

    std::free(slab->memory);
    delete slab;
}

void* SlabCache::allocate()
{
    // 1) Try partial slab first
    if(!partial_slabs_.empty())
    {
        Slab* slab = partial_slabs_.front();
        void* ptr = allocate_from_slab(slab);

        if(slab->free_count == 0)
        {
            partial_slabs_.pop_front();
            full_slabs_.push_back(slab);
        }

        return ptr;
    }

    // 2) Try empty slabs next
    if(!empty_slabs_.empty())
    {
        Slab* slab = empty_slabs_.front();
        empty_slabs_.pop_front();
        partial_slabs_.push_back(slab);

        return allocate_from_slab(slab);
    }

    // 3) Create a new slab
    Slab* slab = create_slab();
    partial_slabs_.push_back(slab);

    return allocate_from_slab(slab);
}

void* SlabCache::allocate_from_slab(Slab* slab)
{
    for(std::size_t i = 0; i < objects_per_slab_; ++i) {
        if(slab->bitmap[i] == 1)
        {
            slab->bitmap[i] = 0;
            slab->free_count--;

            std::byte* slot = slab->memory + i * object_size_;
            return static_cast<void*>(slot);
        }
    }

    return nullptr;
}

void SlabCache::deallocate(void* ptr) 
{
    if(!ptr) return;

    Slab* slab = find_slab_containing(ptr);
    assert(slab && "Pointer doesn't belong to any slab in this cache");

    std::size_t index = (static_cast<std::byte*>(ptr) - slab->memory) / object_size_;

    if(dtor_) dtor_(ptr);

    slab->bitmap[index] = 1;
    slab->free_count++;

    assert(slab->free_count <= objects_per_slab_);

    if (slab->free_count == objects_per_slab_)
    {
        move_to_empty(slab);
    }
    else if (slab->free_count == 1)
    {
        move_to_partial(slab);
    }
}

Slab* SlabCache::find_slab_containing(void* ptr)
{
    auto belongs = [&](Slab* s)
    {
        std::byte* start = s->memory;
        std::byte* end   = start + slab_size_;
        return (static_cast<std::byte*>(ptr) >= start &&
                    static_cast<std::byte*>(ptr) <  end);
    };

    for (auto* s : partial_slabs_) if (belongs(s)) return s;
    for (auto* s : full_slabs_)    if (belongs(s)) return s;
    for (auto* s : empty_slabs_)   if (belongs(s)) return s;

    return nullptr;
}

inline void SlabCache::move_to_empty(Slab* slab)
{
    partial_slabs_.remove(slab);
    full_slabs_.remove(slab);
    empty_slabs_.remove(slab);
    empty_slabs_.push_back(slab);
}

void SlabCache::move_to_partial(Slab* slab)
{
    empty_slabs_.remove(slab);
    full_slabs_.remove(slab);
    partial_slabs_.remove(slab); // IMPORTANT
    partial_slabs_.push_back(slab);
}
