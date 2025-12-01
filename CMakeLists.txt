cmake_minimum_required(VERSION 3.15)
project(cpp_allocators VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(allocators
    src/memory_pool.cpp
    src/slab_allocator.cpp
    src/arena_allocator.cpp
    src/monotonic_allocator.cpp
    src/stack_allocator.cpp
    src/cache_slab_allocator.cpp
)

target_include_directories(allocators
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_executable(example_pool examples/example_pool.cpp)
target_link_libraries(example_pool allocators)

add_executable(example_slab examples/example_slab.cpp)
target_link_libraries(example_slab allocators)

add_executable(example_arena examples/example_arena.cpp)
target_link_libraries(example_arena allocators)

add_executable(example_monotonic examples/example_monotonic.cpp)
target_link_libraries(example_monotonic allocators)

add_executable(example_stack examples/example_stack.cpp)
target_link_libraries(example_stack allocators)

add_executable(example_slab_cache_basic examples/example_slab_cache_basic.cpp)
target_link_libraries(example_slab_cache_basic allocators)

add_executable(example_slab_cache_ctor_dtor examples/example_slab_cache_ctor_dtor.cpp)
target_link_libraries(example_slab_cache_ctor_dtor allocators)

install(TARGETS allocators
    EXPORT allocatorsTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT allocatorsTargets
    FILE allocatorsConfig.cmake
    NAMESPACE alloc::
    DESTINATION lib/cmake/allocators
)
