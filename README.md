# cpp-allocators

A collection of high-performance C++ memory allocators designed for low-latency systems such as:
- trading engines  
- game engines  
- LLM inference pipelines  
- real-time services  

This library provides a clean and modern C++ implementation of custom allocators with a focus on:
- predictable latency  
- efficient small object allocation  
- avoidance of heap fragmentation  
- fully O(1) allocations/deallocations in the hot path  

---

# 🚀 Allocators Included

## **1. Fixed Memory Pool**
A fast allocator for fixed-size objects.

**Features:**  
- O(1) allocate / deallocate  
- Chunk-based growth  
- Pointer-recycling freelist  
- Zero fragmentation  
- Ideal for many objects of identical size  


## **2. Slab Allocator**
A general small-object allocator using power-of-two size classes:

**Size classes:**  
`8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096`

**Features:**  
- Automatic size-class selection  
- Lazy initialization of pools  
- Backed by multiple fixed pools  
- Great for variable small-size allocations  
- Predictable O(1) allocation behavior  


## **3. Arena Allocator (Bump Allocator)**
The simplest and fastest allocation strategy.

**Features:**  
- Linear bump-pointer allocation (O(1))  
- Zero per-object free  
- Instant reset-all semantics  
- Zero fragmentation, strong locality  
- Perfect for short-lived bursts of allocations  


## **4. Monotonic Allocator**
A smarter, growing version of the Arena Allocator.

**Features:**  
- Arena allocation + automatic block expansion  
- Never frees individual objects  
- Bulk reset semantics  
- No fixed-capacity limit  
- Ideal for request-scoped memory, parsers, batch processing  

*(Lightweight alternative to `std::pmr::monotonic_buffer_resource`.)*


## **5. Stack Allocator (LIFO Allocator)**
Arena-style allocator with push/pop markers.

**Features:**  
- LIFO allocation model  
- `push()` → save marker  
- `pop(marker)` → free everything allocated after marker  
- O(1) pop  
- Zero fragmentation  
- Perfect for nested scopes, temporary structures, recursive algorithms  


### More allocators coming soon:
- Freelist Allocator  
- Buddy Allocator  
- Thread-Local Allocators  
- Hybrid Allocators  
- PMR memory_resource wrappers  

Stay tuned!


---

## 📦 Installation & Build (CMake)

```bash
mkdir build
cd build
cmake ..
make
```

### Build Output

After running CMake and Make, the build directory will contain:

- **`liballocators.a`** — the static library containing all allocator implementations  
- **Example executables** (if enabled in the CMakeLists):
  - `example_pool`
  - `example_slab`

---
## 🧪 Usage Examples
### Fixed Memory Pool
```cpp
MemoryPool pool(sizeof(Foo), 1024);

void* mem = pool.allocate();
Foo* f = new (mem) Foo(42);

// ...

f->~Foo();
pool.deallocate(f);

```
### Slab Allocator
```cpp
SlabAllocator slab;

void* p = slab.allocate(60);   // picks 64-byte class
slab.deallocate(p, 60);

```
---
## 🔧 CMake Integration (for other projects)
```cmake
find_package(allocators REQUIRED)
target_link_libraries(my_app alloc::allocators)
```
---
## 📌 Roadmap

Upcoming allocators:

- **Monotonic / Arena Allocator**
- **Stack Allocator**
- **PMR-compatible `memory_resource` wrappers**
- **Thread-local Slab Allocator**
- **Benchmark Suite**
