// ELF test-only failure injection at the actual fast_io C allocator boundary.
// Link with --wrap=malloc,--wrap=calloc,--wrap=realloc. External shared LLVM
// allocation remains external; no production allocator hook or hot-path branch.
#pragma once
#include <cstddef>
#include <cerrno>
namespace fast_io_allocation_probe
{
    inline thread_local bool armed{}, once{};
    inline thread_local std::size_t remaining{}, successful{}, failures{}, selected_size{};
    inline bool reject(std::size_t size) noexcept
    {
        if(!armed || (selected_size && selected_size!=size))return false;
        if(remaining) { --remaining;++successful;return false; }
        ++failures;if(once)armed=false;errno=ENOMEM;return true;
    }
}
extern "C" void* __real_malloc(std::size_t) noexcept;
extern "C" void* __real_calloc(std::size_t,std::size_t) noexcept;
extern "C" void* __real_realloc(void*,std::size_t) noexcept;
extern "C" void* __wrap_malloc(std::size_t bytes) noexcept
{ return fast_io_allocation_probe::reject(bytes) ? nullptr : __real_malloc(bytes); }
extern "C" void* __wrap_calloc(std::size_t count,std::size_t width) noexcept
{
    if(width && count>static_cast<std::size_t>(-1)/width) { errno=ENOMEM;return nullptr; }
    return fast_io_allocation_probe::reject(count*width) ? nullptr : __real_calloc(count,width);
}
extern "C" void* __wrap_realloc(void* pointer,std::size_t bytes) noexcept
{ return fast_io_allocation_probe::reject(bytes) ? nullptr : __real_realloc(pointer,bytes); }
