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

## 🚀 Allocators Included

### **1. Fixed Memory Pool**
A fast allocator for fixed-size objects:
- O(1) allocate/deallocate  
- chunk-based growth  
- pointer-recycling freelist  
- perfect for objects of identical size  

### **2. Slab Allocator**
A general small-object allocator with power-of-two size classes:
8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 bytes

Features:
- lazy initialization of pools  
- automatic size-class selection  
- ideal for variable small-size allocations  

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
